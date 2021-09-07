import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import com.jolla.keyboard 1.0
import xyz.birdzhang.ime 1.0
import xyz.birdzhang.opencc 1.0

InputHandler {
    id: handler
    property string preedit
    property var trie
    property bool trie_built: false
    property var regLetter : /^[A-Za-z]+$/
    property int candidateSpaceIndex: -1
    property int maxsize: 80

    property bool composingEnabled: !keyboard.inSymView
    property bool hasMore: composingEnabled && gpy.hasMore
    //mod start
    property bool pinyinMode: true
    property int cursorIndex: MInputMethodQuick.cursorPosition

    ConfigurationGroup{
        id: config
        path: "/app/xyz.birdzhang.ime"
        property int pageSize: 20
        property int fetchSize: 15
        property bool traditional: false
        property string convertModel: "s2twp"
    }

    onPinyinModeChanged: {
        handler.composingEnabled = handler.pinyinMode
        keyboard.layout.pinyinMode = handler.pinyinMode
        if (handler.preedit != "") {
            //commit(handler.preedit)
        }
        reset()
    }

    onCursorIndexChanged: {
        if(preedit == "" ){
            getPredictions(false);
        }
    }

    onActiveChanged: {
        if (active) {
            if(pinyinMode){
                getPredictions(false);
                opencc.chooseMode(config.convertModel);
                opencc2s.chooseMode(opencc2s.revert(config.convertModel));
            }
            keyboard.layout.pinyinMode = handler.pinyinMode
            keyboard.shiftKeyPressed = false
            // keyboard.cycleShift();
            keyboard.shiftState = ShiftState.NoShift;
            MInputMethodQuick.sendCommit("");

        } else if(!pinyinMode && preedit != ""){
            commit(preedit);
        } else {
            handler.clearPreedit()
        }
    }

    OpenCC{
        id: opencc
        function convert2s(str){
            if(!str)return "";
            return opencc2s.convert(str);
        }
    }
    OpenCC{
        id: opencc2s
        function revert(model){
            model = model.replace("p","");
            var models = model.split("2");
            return models[1]+"2"+ models[0];
        }

    }

    QmlPinyin{
        id :gpy
        property var candidates: ListModel { }
        property var moreCandidates: ListModel { }
        property var pySqlStart: []
        property var olderSql
        property bool hasMore:false
        property bool fetchMany:false
        property int pageSize: config.pageSize;
        property int fetchSize: config.fetchSize;
        property int pred:0

        signal candidatesUpdated

        Component.onCompleted:{
            console.log("pageSize:"+config.pageSize)
            gpy.init();
            //gpy.setUserDictionary(true);
            
        }

        function getMoreCandidates(){
            moreCandidates.clear();
            for (var i = pageSize; i < pred ; i++) {
                moreCandidates.append({
                          text: config.traditional? opencc.convert(gpy.candidateAt(i)) :gpy.candidateAt(i),
                          type: "partial",
                          segment: 0,
                          candidate: i
                         })
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
                candidates.append({
                          text: config.traditional? opencc.convert(gpy.candidateAt(i)) :gpy.candidateAt(i),
                          type: "full",
                          segment: 0,
                          candidate: i
                      })
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
                // 没有全部选择
                if(!flag){
                    var tmpPy = gpy.pinyinString(false)

                    var tmpSubPy = tmpPy.slice(parseInt(pySqlStart[fixLen+1]), tmpPy.length)

		            if(tmpPy == tmpSubPy){
                      	MInputMethodQuick.sendCommit(item.text) 
                        handler.preedit = "" 
                        return;
                    }
        			handler.preedit = ""
                    commit_partial(item.text, tmpSubPy)
                    if(hasMore && fetchMany){
                        hasMore = false
                        fetchMany = false
                        moreCandidates.clear()
                    }
                    candidates.clear()


                    if(pred > pageSize){
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

            }else{
                console.log("+++++++++",handler.preedit)
            }
        }
    }

    topItem: Component {
        Column{
            id: topItem
            width: parent  ? parent.width : 0
            TopItem {
//                visible:  !keyboard.inSymView
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
                        onClicked: selectPhrase(model.text, model.index)
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
                    onCountChanged: {
                        positionViewAtBeginning();
                    }
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
            }
        }
    }

    Component {
        id: pasteComponent
        PasteButton {
            visible: Clipboard.text
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
                        onClicked: selectPhrase(model.text, model.index)

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

            property int inactivePadding: Theme.paddingMedium

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
                    visible: handler.hasMore

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
                    id: background
                    onClicked: selectPhrase(model.text, model.index)
                    width: parent.width
                    // height: geometry.keyHeightLandscape // assuming landscape!
                    height: geometry.keyHeightLandscape * candidateText.lineCount

                    Text {
                        id: candidateText
                        width: background.width
                        horizontalAlignment: Text.AlignHCenter
                        anchors.verticalCenter: parent.verticalCenter
                        // color: index === 0 ? Theme.highlightColor : Theme.primaryColor
                        // font.pixelSize: Theme.fontSizeSmall
                        // fontSizeMode: Text.HorizontalFit
                        color: (background.down || (index === 0 && preedit.length > 0))
                               ? Theme.highlightColor : Theme.primaryColor
                        font { pixelSize: Theme.fontSizeSmall; family: Theme.fontFamily }
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
        } else if (pressedKey.key === Qt.Key_Backspace && preedit == "") {
//                getPredictions(true);
        } else if (pressedKey.key === Qt.Key_Home) {
            MInputMethodQuick.sendKey(Qt.Key_Home, 0, "", Maliit.KeyClick)
        } else if (pressedKey.key === Qt.Key_End) {
            MInputMethodQuick.sendKey(Qt.Key_End, 0, "", Maliit.KeyClick)
        } else if (pressedKey.key === Qt.Key_Up) {
            MInputMethodQuick.sendKey(Qt.Key_Up, 0, "", Maliit.KeyClick)
        } else if (pressedKey.key === Qt.Key_Down) {
            MInputMethodQuick.sendKey(Qt.Key_Down, 0, "", Maliit.KeyClick)
        } else if (pressedKey.key === Qt.Key_Left) {
            MInputMethodQuick.sendKey(Qt.Key_Left, 0, "", Maliit.KeyClick)
        } else if (pressedKey.key === Qt.Key_Right) {
            MInputMethodQuick.sendKey(Qt.Key_Right, 0, "", Maliit.KeyClick)
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
        gpy.acceptPhrase(index);
    }


    function getPredictions(isDelete){
        preedit = "";
        commit(preedit)
        gpy.candidates.clear();
        var tmppredictionsList = [];
        // if config.traditional, need convert t2s
        if(!isDelete){
            var preText = MInputMethodQuick.surroundingText.substring(MInputMethodQuick.cursorPosition-1,
                                                                      MInputMethodQuick.cursorPosition);
            if(config.traditional)preText = opencc.convert2s(preText);
            tmppredictionsList = gpy.predictionList(preText,
                                    gpy.fetchSize);
        }else{
            var preDelText = MInputMethodQuick.surroundingText.substring(MInputMethodQuick.cursorPosition-2,
                                                                         MInputMethodQuick.cursorPosition-1);
            if(config.traditional)preDelText =  opencc.convert2s(preDelText);
            tmppredictionsList = MInputMethodQuick.surroundingText.length > 2 ?
                        gpy.predictionList(preDelText,
                                           gpy.fetchSize):[]
        }
        // end
        for (var i = 0; i < tmppredictionsList.length; i++) {
            gpy.candidates.append({
                            text: config.traditional? opencc.convert(tmppredictionsList[i]): tmppredictionsList[i],
                            type: "full",
                            segment: 0,
                            candidate: i
                           })
        }
        if(tmppredictionsList.length > 0){
            gpy.candidates.append({text: " ", type: "full", segment: 0, candidate: tmppredictionsList.length});
        }
        gpy.hasMore = false;
        gpy.candidatesUpdated();
    }

    function isInputCharacter(character) {
        return "\'-".indexOf(character) >= 0
    }
    function applyPrediction(replacement, index) {
        handler.preedit = "";
        if(pressedKey.text && pressedKey.text.length > 0){
            pressedKey.text = "";
        }

        replacement = replacement + " "
        candidateSpaceIndex = MInputMethodQuick.surroundingTextValid
                ? MInputMethodQuick.cursorPosition + replacement.length : -1
        commit(replacement);
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
        handler.preedit = ""
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
        handler.preedit = "";
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

