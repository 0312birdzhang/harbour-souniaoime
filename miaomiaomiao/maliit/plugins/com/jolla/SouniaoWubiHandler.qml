import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import com.jolla.keyboard 1.0
import xyz.birdzhang.wubi 1.0

InputHandler {
    id: handler

    property string preedit: ""
    property var regLetter: /^[a-zA-Z]+$/
    property bool composingEnabled: !keyboard.inSymView
    property bool hasMore: false
    property string lastCommitted: ""

    onPreeditChanged: MInputMethodQuick.sendPreedit(preedit)

    WubiRecognition {
        id: wubiRecognition
        version: wubiConfig.wubiVersion
    }

    ConfigurationGroup {
        id: wubiConfig
        path: "/app/xyz.birdzhang.ime"
        property string wubiVersion: "wubi86"
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
                id: listTopItem
                width: parent.width

                SilicaListView {
                    id: listView
                    model: wubiRecognition.candidates
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
                model: wubiRecognition.candidates
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
            if (preedit !== "" && wubiRecognition.candidates.length > 0) {
                accept(0)
                handled = true
            }
        } else if (pressedKey.key === Qt.Key_Return) {
            if (preedit !== "") {
                commit(preedit)
                handled = true
            }
        } else if (pressedKey.key === Qt.Key_Backspace && preedit !== "") {
            wubiRecognition.backspace()
            preedit = wubiRecognition.currentInput
            handled = true
        } else if (pressedKey.text.length !== 0) {
            if (!keyboard.inSymView && regLetter.test(pressedKey.text)) {
                wubiRecognition.input(pressedKey.text)
                preedit = wubiRecognition.currentInput
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

    function accept(index) {
        if (index < 0 || index >= wubiRecognition.candidates.length)
            return
        var text = wubiRecognition.pick(index)
        if (text && text.length > 0) {
            lastCommitted = text
            MInputMethodQuick.sendCommit(text)
            preedit = wubiRecognition.currentInput
        }
    }

    function clearPreedit() {
        preedit = ""
        wubiRecognition.clear()
        MInputMethodQuick.sendPreedit("")
    }

    function commit(text) {
        lastCommitted = text
        MInputMethodQuick.sendCommit(text)
        clearPreedit()
    }

    function reset() {
        clearPreedit()
        lastCommitted = ""
        if (keyboard.shiftState !== ShiftState.LockedShift) {
            keyboard.shiftState = ShiftState.NoShift
        }
    }
}
