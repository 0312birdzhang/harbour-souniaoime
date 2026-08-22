import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import com.jolla.keyboard 1.0
import xyz.birdzhang.stroke 1.0
import xyz.birdzhang.ime 1.0

InputHandler {
    id: handler

    property string preedit: ""
    property var regStroke: /^[1-5]$/
    property var strokeToCode: ({ "一": "1", "丨": "2", "丿": "3", "丶": "4", "乛": "5" })
    property bool composingEnabled: !keyboard.inSymView
    property bool hasMore: false
    property string lastCommitted: ""
    property bool predictionMode: false
    property var predictionCandidates: []

    StrokeRecognition {
        id: strokeRecognition
    }

    QmlPinyin {
        id: gpy
        Component.onCompleted: gpy.init()
        Component.onDestruction: gpy.flushCache()
    }

    onActiveChanged: {
        if (active) {
            reset()
            keyboard.layout.pinyinMode = true
            keyboard.shiftKeyPressed = false
            keyboard.shiftState = ShiftState.NoShift
            MInputMethodQuick.sendCommit("")
        } else {
            clearPreedit()
        }
    }

    topItem: Component {
        Column {
            id: topItem
            width: parent ? parent.width : 0

            TopItem {
                visible: false
                width: parent.width

                Rectangle {
                    anchors.fill: parent
                    color: Theme.highlightBackgroundColor
                    opacity: .05
                }

                Label {
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    text: preedit
                }

                MouseArea {
                    anchors.fill: parent
                }
            }

            TopItem {
                id: listTopItem
                width: parent.width

                SilicaListView {
                    id: listView
                    model: predictionMode ? predictionCandidates : strokeRecognition.candidates
                    orientation: ListView.Horizontal
                    width: parent.width
                    height: parent.height
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    header: pasteComponent

                    delegate: BackgroundItem {
                        id: background
                        width: candidateText.width + Theme.paddingLarge * 2
                        height: listTopItem.height
                        onClicked: accept(index)

                        Text {
                            id: candidateText
                            anchors.centerIn: parent
                            color: (background.down || index === 0) ? Theme.highlightColor : Theme.primaryColor
                            font { pixelSize: Theme.fontSizeSmall; family: Theme.fontFamily }
                            text: modelData
                        }
                    }

                    onCountChanged: positionViewAtBeginning()
                }
            }
        }
    }

    Component {
        id: pasteComponent
        PasteButton {
            visible: Clipboard.text
            onClicked: {
                clearPreedit()
                MInputMethodQuick.sendCommit(Clipboard.text)
                keyboard.expandedPaste = false
            }
        }
    }

    verticalItem: Component {
        Item {
            id: verticalContainer
            property int inactivePadding: Theme.paddingMedium

            SilicaListView {
                id: verticalList
                model: predictionMode ? predictionCandidates : strokeRecognition.candidates
                anchors.fill: parent
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                header: Component {
                    PasteButtonVertical {
                        visible: Clipboard.hasText
                        width: verticalList.width
                        height: visible ? geometry.keyHeightLandscape : 0
                        popupParent: verticalContainer
                        popupAnchor: 2

                        onClicked: {
                            clearPreedit()
                            MInputMethodQuick.sendCommit(Clipboard.text)
                        }
                    }
                }

                delegate: BackgroundItem {
                    id: background
                    width: parent.width
                    height: geometry.keyHeightLandscape * candidateText.lineCount
                    onClicked: accept(index)

                    Text {
                        id: candidateText
                        width: background.width
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: (background.down || index === 0) ? Theme.highlightColor : Theme.primaryColor
                        font { pixelSize: Theme.fontSizeSmall; family: Theme.fontFamily }
                        text: modelData
                        wrapMode: Text.Wrap
                        maximumLineCount: 2
                    }
                }

                MouseArea {
                    height: parent.height
                    width: verticalContainer.inactivePadding
                }

                MouseArea {
                    height: parent.height
                    width: verticalContainer.inactivePadding
                    anchors.right: parent.right
                }
            }
        }
    }

    function handleKeyClick() {
        var handled = false
        keyboard.expandedPaste = false

        if (pressedKey.key === Qt.Key_Space) {
            if (preedit !== "" && strokeRecognition.candidates.length > 0) {
                accept(0)
                handled = true
            } else {
                MInputMethodQuick.sendCommit(" ")
                handled = true
            }
        } else if (pressedKey.key === Qt.Key_Return) {
            if (preedit !== "") {
                clearPreedit()
                handled = true
            }
        } else if (pressedKey.key === Qt.Key_Backspace && preedit !== "") {
            preedit = preedit.slice(0, preedit.length - 1)
            updateCandidates()
            handled = true
        } else if (pressedKey.text === "符号") {
            keyboard.inSymView = !keyboard.inSymView
            handled = true
        } else if (pressedKey.text === "笔画") {
            keyboard.inSymView = false
            handled = true
        } else if (pressedKey.text.length !== 0) {
            if (!keyboard.inSymView && isStrokeKey(pressedKey.text)) {
                preedit += pressedKey.text
                updateCandidates()
                handled = true
            } else {
                clearPreedit()
                MInputMethodQuick.sendCommit(pressedKey.text)
                handled = true
            }
        }

        if (handled && keyboard.shiftState !== ShiftState.LockedShift) {
            keyboard.shiftState = ShiftState.NoShift
        }

        return handled
    }

    function isStrokeKey(text) {
        return text === "一" || text === "丨" || text === "丿" || text === "丶" || text === "乛"
    }

    function strokeToCodes(strokes) {
        var result = ""
        for (var i = 0; i < strokes.length; i++) {
            result += strokeToCode[strokes[i]] || ""
        }
        return result
    }

    function updateCandidates() {
        predictionMode = false
        if (preedit.length > 0) {
            strokeRecognition.recognize(strokeToCodes(preedit))
            MInputMethodQuick.sendPreedit(preedit)
        } else {
            strokeRecognition.recognize("")
            MInputMethodQuick.sendPreedit("")
        }
    }

    function updatePredictions(history) {
        predictionCandidates = []
        if (!history || history.length === 0) {
            predictionMode = false
            return
        }
        var result = []
        var list = gpy.predictionList(history, 20)
        for (var i = 0; i < list.length; i++) {
            if (list[i] && list[i].length > 0)
                result.push(list[i])
        }
        predictionCandidates = result
        predictionMode = predictionCandidates.length > 0
    }

    function accept(index) {
        if (predictionMode) {
            if (index >= 0 && index < predictionCandidates.length) {
                commitPrediction(predictionCandidates[index])
            }
            return
        }
        var text = strokeRecognition.pick(index)
        if (text.length > 0) {
            commitStroke(text)
        }
    }

    function clearPreedit() {
        preedit = ""
        strokeRecognition.recognize("")
        MInputMethodQuick.sendPreedit("")
    }

    function clearPredictions() {
        predictionCandidates = []
        predictionMode = false
    }

    function reset() {
        clearPreedit()
        clearPredictions()
        lastCommitted = ""
        if (keyboard.shiftState !== ShiftState.LockedShift) {
            keyboard.shiftState = ShiftState.NoShift
        }
    }

    function commitStroke(text) {
        clearPreedit()
        lastCommitted = text
        MInputMethodQuick.sendCommit(text)
        updatePredictions(text)
    }

    function commitPrediction(text) {
        clearPreedit()
        lastCommitted = text
        MInputMethodQuick.sendCommit(text)
        updatePredictions(text)
    }

    function commit(text) {
        clearPreedit()
        clearPredictions()
        MInputMethodQuick.sendCommit(text)
    }
}
