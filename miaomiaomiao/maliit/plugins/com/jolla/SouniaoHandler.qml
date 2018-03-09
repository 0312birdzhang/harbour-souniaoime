import QtQuick 2.0
import Sailfish.Silica 1.0
import com.meego.maliitquick 1.0
import com.jolla.keyboard 1.0
import xyz.birdzhang.ime 1.0

InputHandler {
    id: handler
    property string preedit
    property var trie
    property bool trie_built: false
    property var regLetter : /^[A-Za-z]+$/
    property int candidateSpaceIndex: -1

    property bool composingEnabled: !keyboard.inSymView
    property bool hasMore: composingEnabled && gpy.hasMore
    //mod start
    property bool pinyinMode: MInputMethodQuick.contentType !== Maliit.UrlContentType && MInputMethodQuick.contentType !== Maliit.EmailContentType


    onPinyinModeChanged: {
        handler.composingEnabled = handler.pinyinMode
        keyboard.layout.pinyinMode = handler.pinyinMode
        if (handler.preedit != "") {
            commit(handler.preedit)
        }
        reset()
    }

    onActiveChanged: {
        if (active) {
            if(pinyinMode){
                gpy.update_candidates(MInputMethodQuick.surroundingText.substring(0, MInputMethodQuick.cursorPosition))
            }
            keyboard.layout.pinyinMode = handler.pinyinMode
            keyboard.shiftKeyPressed = false
            keyboard.cycleShift()
            MInputMethodQuick.sendCommit("")
        } else if(!pinyinMode && preedit != ""){
            commit(preedit);
        } else {
            handler.clearPreedit()
        }
    }

   
    QmlPinyin{
        id :gpy
        property var candidates: ListModel { }
        property var moreCandidates: ListModel { }
        property var pySqlStart
        property var olderSql
        property bool hasMore:false
        property bool fetchMany:false
        property int pageSize:20
        property int pred:0

        signal candidatesUpdated

        Component.onCompleted:{
            gpy.init();
            gpy.setUserDictionary(true);
        }

        function getMoreCandidates(){
            for (var i = pageSize; i < pred ; i++) {
                moreCandidates.append({text: gpy.candidateAt(i), type: "partial", segment: 0, candidate: i})
            }
            fetchMany = true
        }

        function update_candidates(str) {
            candidates.clear()
            moreCandidates.clear()
            fetchMany = false
            hasMore = false
            pred = gpy.search(str)
            pySqlStart = gpy.spellingStartPositions()
            olderSql = str
            if(pred > pageSize)
            {
                hasMore = true

            }else{
                hasMore = false
            }
            for (var i = 0; i < pred && i < pageSize; i++) {
                candidates.append({text: gpy.candidateAt(i), type: "full", segment: 0, candidate: i})
            }
            candidatesUpdated()
        }

        function acceptPhrase(index, pr) {
            var fixLen = -1
            var nSize = -1
            var item = ""
            if(hasMore && fetchMany){
                item = moreCandidates.get(index)
                nSize = gpy.chooceCandidate(index+pageSize)
                fixLen = gpy.fixedLength()
                pred = nSize
            }else{
                item = candidates.get(index)
                nSize = gpy.chooceCandidate(index)
                fixLen  = gpy.fixedLength()
                pred = nSize
            }


            var flag = false

            if(fixLen == parseInt(pySqlStart[0]))
            {
                flag = true
            }else{
                flag = false
            }
            if (item.text != ""){

                if(!flag){
                    var tmpPy = gpy.pinyinString(false)

                    var tmpSubPy = tmpPy.slice(parseInt(pySqlStart[fixLen+1]), tmpPy.length)

                    MInputMethodQuick.sendCommit(item.text)
                    MInputMethodQuick.sendPreedit( tmpSubPy )
                    preedit = tmpSubPy

                    if(hasMore && fetchMany){

                        hasMore = false
                        fetchMany = false
                        //moreCandidates.clean()
                    }
                    candidates.clear()


                    if(pred > pageSize)
                    {
                        hasMore = true

                    }else{
                        hasMore = false
                    }


                    for (var i = 0; i < nSize && i < pageSize; i++) {
                        candidates.append({text: gpy.candidateAt(i), type: "partial", segment: 0, candidate: i})
                    }
                    candidatesUpdated()

                }else{
                    commit(item.text)
                }

            }
        }
    }

    topItem: Component {
        Column{
            id: topItem
            width: parent  ? parent.width : 0
            TopItem {
                // visible:  !keyboard.inSymView
                visible: false
                width: parent.width
                Rectangle {
                    id: background
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
                    model: gpy.candidates
                    orientation: ListView.Horizontal
                    width: parent.width
                    height: parent.height
                    boundsBehavior: ((!keyboard.expandedPaste && Clipboard.hasText) || gpy.hasMore) ? Flickable.DragOverBounds : Flickable.StopAtBounds
                    header: pasteComponent

                    footer: Item {
                        width: gpy.hasMore ? 20 : 0
                        height: listView.height
                        visible: gpy.hasMore

                        Image {
                            source: "image://theme/icon-lock-more"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Image {
                            source: "image://theme/icon-lock-more?" + Theme.highlightColor
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: listView.dragging && listView.atXEnd ? 1.0 : 0.0
                            Behavior on opacity { FadeAnimation {} }
                        }

                    }

                    delegate: BackgroundItem {
                        id: backGround
                        // onClicked:  accept(model.index)
                        onClicked: pinyinMode ? selectPhrase(model.text, model.index) : applyPrediction(model.text, model.index)
                        width: candidateText.width + Theme.paddingLarge * 2
                        height: listTopItem.height

                        Text {
                            id: candidateText
                            anchors.centerIn: parent
                            color: (backGround.down || index === 0) ? Theme.highlightColor : Theme.primaryColor
                            font { pixelSize: Theme.fontSizeSmall; family: Theme.fontFamily }
                            text: model.text
                        }
                    }
                    onCountChanged: positionViewAtBeginning()
                    onDraggingChanged: {

                        if (!dragging) {
                            if (!keyboard.expandedPaste && contentX < -(headerItem.width + Theme.paddingLarge)) {
                                keyboard.expandedPaste = true
                                positionViewAtBeginning()
                            } else if (atXEnd && gpy.hasMore) {
                                gpy.fetchMany=true
                                gpy.getMoreCandidates()

                            }
                        }
                    }

                    Binding on flickDeceleration {
                        when: phraseDialog.visible
                        value: 1000000
                    }

                    Connections {
                        target: gpy
                        onCandidatesUpdated: listView.positionViewAtBeginning()
                    }

                    Connections {
                        target: Clipboard
                        onTextChanged: {
                            if (Clipboard.hasText) {
                                // need to have updated width before repositioning view
                                positionerTimer.restart()
                            }
                        }
                    }

                    Timer {
                        id: positionerTimer
                        interval: 10
                        onTriggered: listView.positionViewAtBeginning()
                    }
                }
                Label {
                    id: switchLabel
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter
                    visible: false
                    height: parent.height
                    width: parent.width * 0.11
                    font { pixelSize: Theme.fontSizeSmall; family: Theme.fontFamily }
                    color: Theme.primaryColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    maximumLineCount: 1
                    text: handler.pinyinMode ? "中" : "英"
                    Rectangle {
                        id: switchButton
                        color: Theme.primaryColor
                        opacity: 0.3 //0.17
                        radius: geometry.keyRadius

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: parent.height * 0.8
                        width: parent.width
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                switchLabel.color = Theme.highlightColor
                                switchButton.color = Theme.highlightBackgroundColor
                                switchButton.opacity = 0.6
                            }
                            onReleased: {
                                switchLabel.color = Theme.primaryColor
                                switchButton.color = Theme.primaryColor
                                switchButton.opacity = 0.3
                            }
                            onClicked: {
                                handler.pinyinMode = !handler.pinyinMode
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: pasteComponent
        PasteButton {
            onClicked: {
                if (preedit.length > 0) {
                    commit(preedit)
                }
                MInputMethodQuick.sendCommit(Clipboard.text)
                keyboard.expandedPaste = false
            }
        }
    }




    onComposingEnabledChanged: {
        if (preedit.length > 0) {
            MInputMethodQuick.sendCommit(preedit)
            gpy.resetSearch()
        }
    }


    Rectangle {
        id: phraseDialog

        parent: keyboard
        z: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: inputItems.height

        visible: gpy.fetchMany
        color: Theme.highlightDimmerColor
        opacity: 0.9
        clip: true

        MultiPointTouchArea {
            // prevent events leaking below
            anchors.fill: parent
            z: -1
        }

        SilicaFlickable {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: closeButton.left

            contentHeight: gridView.height

            Flow {
                id: gridView

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.paddingMedium

                property real cellWidth: width / (keyboard.portraitMode ? 5 : 8)

                Repeater {
                    model: phraseDialog.visible ? gpy.moreCandidates : 0
                    delegate: BackgroundItem {
                        id: gridItemBackground
                        height: Theme.itemSizeSmall
                        width: Math.ceil((gridText.contentWidth + 2*Theme.paddingMedium) / gridView.cellWidth)* gridView.cellWidth
                        onClicked: pinyinMode ? selectPhrase(model.text, model.index) : applyPrediction(model.text, model.index) //accept(model.index)

                        Text {
                            id: gridText
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.paddingMedium
                            color: gridItemBackground.down ? Theme.highlightColor : Theme.primaryColor
                            font { pixelSize: Theme.fontSizeSmall; family: Theme.fontFamily }
                            text: model.text
                        }
                    }
                }
            }
        }

        IconButton {
            id: closeButton

            anchors {
                right: parent.right
                top: parent.top
                topMargin: Theme.paddingSmall
            }
            opacity: 0.6
            icon.source: "image://theme/icon-close-vkb"
            onClicked: gpy.fetchMany = false
        }
    }


    verticalItem: Component {
        Item {
            id: verticalContainer

            SilicaListView {
                id: verticalList

                model: composingEnabled ? gpy.candidates : 0
                anchors.fill: parent
                clip: true
                boundsBehavior: handler.hasMore ? Flickable.DragOverBounds : Flickable.StopAtBounds
                header: Component {
                    PasteButtonVertical {
                        visible: Clipboard.hasText
                        width: verticalList.width
                        height: visible ? geometry.keyHeightLandscape : 0
                        popupParent: verticalContainer
                        popupAnchor: 2 // center

                        onClicked: {
                            commit(preedit)
                            MInputMethodQuick.sendCommit(Clipboard.text)
                        }
                    }
                }

                footer: Item {
                    width:  verticalList.width
                    height: geometry.keyHeightLandscape / 2
                    visible: gpy.hasMore

                    Image {
                        id: moreIcon
                        source: "image://theme/icon-lock-more"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.bottom
                        anchors.verticalCenterOffset: -Theme.paddingSmall
                    }
                    Image {
                        source: "image://theme/icon-lock-more?" + Theme.highlightColor
                        anchors.centerIn: moreIcon
                        opacity: verticalList.dragging && verticalList.atYEnd ? 1.0 : 0.0
                        Behavior on opacity { FadeAnimation {} }
                    }
                }



                delegate: BackgroundItem {
                    onClicked: pinyinMode ? selectPhrase(model.text, model.index) : applyPrediction(model.text, model.index) //accept(model.index)
                    width: parent.width
                    height: geometry.keyHeightLandscape // assuming landscape!

                    Text {
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: index === 0 ? Theme.highlightColor : Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        fontSizeMode: Text.HorizontalFit
                        //                        textFormat: Text.StyledText
                        text: model.text
                        wrapMode: Text.Wrap
                        maximumLineCount: 2
                    }
                }

                onDraggingChanged: {
                    if (!dragging && atYEnd && gpy.hasMore) {
                        gpy.getMoreCandidates()
                    }
                }

                Binding on flickDeceleration {
                    when: phraseDialog.visible
                    value: 1000000
                }

                Connections {
                    target: gpy
                    onCandidatesUpdated: {
                        if (!clipboardChange.running) {
                            verticalList.positionViewAtIndex(0, ListView.Beginning)
                        }
                    }
                }
                Connections {
                    target: Clipboard
                    onTextChanged: {
                        verticalList.positionViewAtBeginning()
                        clipboardChange.restart()
                    }
                }
                Timer {
                    id: clipboardChange
                    interval: 1000
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
            if (preedit !== "") {
                accept(0)

                if (keyboard.shiftState !== ShiftState.LockedShift) {
                    keyboard.shiftState = ShiftState.NoShift
                }


                handled = true
            }
        } else if (pressedKey.key === Qt.Key_Return) {
            if (preedit !== "") {
                commit(preedit)

                if (keyboard.shiftState !== ShiftState.LockedShift) {
                    keyboard.shiftState = ShiftState.NoShift
                }

                handled = true
            }
        } else if (pressedKey.key === Qt.Key_Backspace && preedit !== "") {

            preedit = preedit.slice(0, preedit.length-1)
            gpy.update_candidates(preedit)
            MInputMethodQuick.sendPreedit(preedit)
            
            if (keyboard.shiftState !== ShiftState.LockedShift) {
                keyboard.shiftState = ShiftState.NoShift
            }

            handled = true
        } else if (pressedKey.text.length !== 0) {
            if( !regLetter.test(pressedKey.text)){
                MInputMethodQuick.sendCommit(pressedKey.text)
            }else{

                preedit = preedit + pressedKey.text
                gpy.update_candidates(preedit)

                if (keyboard.shiftState !== ShiftState.LockedShift) {
                    keyboard.shiftState = ShiftState.NoShift
                }

                MInputMethodQuick.sendPreedit(preedit)
            }
            handled = true
        }

        return handled
    }

    function accept(index) {
        console.debug("attempting to accept", index)
        gpy.acceptPhrase(index, preedit)
    }

    function selectPhrase(phrase, index) {
        console.log("phrase clicked: " + phrase)
        // MInputMethodQuick.sendCommit(phrase)
        gpy.acceptPhrase(index)
        if (preedit.length > 0 ) {
            MInputMethodQuick.sendPreedit(preedit)
        }
    }

    function applyPrediction(replacement, index) {
        console.log("candidate clicked: " + replacement + "\n")
        replacement = replacement + " "
        candidateSpaceIndex = MInputMethodQuick.surroundingTextValid
                ? MInputMethodQuick.cursorPosition + replacement.length : -1
        commit(replacement)
        // thread.acceptPrediction(index)
    }

    function selectPhraseAndShrink(phrase, index) {
        selectPhrase(phrase, index)
        gpy.fetchMany = false
    }

    function clearPreedit(){
        if(preedit.length > 0){
            MInputMethodQuick.sendCommit(preedit)
            gpy.resetSearch()
        }
        handled.preedit = ""
    }
    function reset() {
        gpy.candidates.clear()
        preedit = ""
        gpy.resetSearch()
        gpy.candidatesUpdated()
        gpy.moreCandidates.clear()
        gpy.fetchMany = false
        gpy.hasMore = false
        
        if (keyboard.shiftState !== ShiftState.LockedShift) {
            keyboard.shiftState = ShiftState.NoShift
        }

    }

    function commit(text) {
        MInputMethodQuick.sendCommit(text)
        reset()
    }

    function commit_partial(text, pe) {
        MInputMethodQuick.sendCommit(text)
        preedit = pe
        MInputMethodQuick.sendPreedit(preedit)
        gpy.update_candidates(preedit)
    }
}
