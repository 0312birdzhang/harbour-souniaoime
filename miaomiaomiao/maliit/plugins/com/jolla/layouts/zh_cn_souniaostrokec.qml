import QtQuick 2.0
import ".."

KeyboardLayout {
    type: "china_stroke"
    inputMode: "stroke"
    property bool pinyinMode: true
    property int strokeKeyWidth: (parent.width) / 5
    
    KeyboardRow {
        visible: keyboard.inSymView

        CharacterKey { symView: "1"; symView2: "@" }
        CharacterKey { symView: "2"; symView2: "/" }
        CharacterKey { symView: "3"; symView2: "\\" }
        CharacterKey { symView: "4"; symView2: "~" }
        CharacterKey { symView: "5"; symView2: "^" }
        CharacterKey { symView: "6"; symView2: "_" }
        CharacterKey { symView: "7"; symView2: "¥" }
        CharacterKey { symView: "8"; symView2: "€" }
        CharacterKey { symView: "9"; symView2: "$" }
        CharacterKey { symView: "0"; symView2: "£" }
    }

    KeyboardRow {
        visible: keyboard.inSymView

        CharacterKey { symView: "*"; symView2: "§" }
        CharacterKey { symView: "#"; symView2: "=" }
        CharacterKey { symView: "+"; symView2: "〈" }
        CharacterKey { symView: "-"; symView2: "〉" }
        CharacterKey { symView: "（"; symView2: "(" }
        CharacterKey { symView: "）"; symView2: ")" }
        CharacterKey { symView: "—"; symView2: "《" }
        CharacterKey { symView: "…"; symView2: "》" }
        CharacterKey { symView: "%"; symView2: "&" }
        CharacterKey { symView: "'"; symView2: "\"" }
    }

    KeyboardRow {
        id: centerRow
        visible: keyboard.inSymView

        ShiftKey {}

        CharacterKey { symView: "。"; symView2: "“" }
        CharacterKey { symView: "，"; symView2: "”" }
        CharacterKey { symView: "；"; symView2: ";" }
        CharacterKey { symView: "："; symView2: ":" }
        CharacterKey { symView: "、"; symView2: "·" }
        CharacterKey { symView: "！"; symView2: "!" }
        CharacterKey { symView: "？"; symView2: "?" }

        BackspaceKey {}
    }


    //Row {
        ////width: parent.width
        //height: keyHeight * 2
        //visible: !keyboard.inSymView
        //anchors.horizontalCenter: parent.horizontalCenter
        //ListView {
            //id: customized_list
            //width: strokeKeyWidth
            //height: keyHeight * 2
            //clip: true
            //model: ListModel {
                //ListElement {character: "！"}
                //ListElement {character: "。"}
                //ListElement {character: "？"}
                //ListElement {character: "："}
                //ListElement {character: "."}
            //}
            //delegate: CharacterKey {
                //width: strokeKeyWidth
                //height: keyHeight * 0.5
                //implicitSeparator: false
                //caption: character
                //captionShifted: character
            //}
            //Rectangle {
                //height: 1
                //width: customized_list.width * 0.5
                //color: Theme.primaryColor
                //anchors.verticalCenter: customized_list.bottom
                //anchors.horizontalCenter: customized_list.horizontalCenter
            //}
        //}
        //Column {
            //width: strokeRow1.width
            ////height: keyHeight * 2
            //Row {
                //id: strokeRow1
                ////property int strokeKeyWidth: (parent.width - backspace.width) / 5

                //height: keyHeight
                //visible: !keyboard.inSymView
                ////anchors.horizontalCenter: parent.horizontalCenter

                //CharacterKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "\u4E00"
                    //captionShifted: "1"
                //}
                //CharacterKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "\u4E28"
                    //captionShifted: "2"
                //}
                //CharacterKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "\u4E3F"
                    //captionShifted: "3"
                //}

                //BackspaceKey {
                    //id: backspace
                    //height: keyHeight
                    //width: strokeKeyWidth
                //}
            //}
            //Row {
                //id: strokeRow2
                ////property int strokeKeyWidth: (parent.width - backspace.width) / 6

                //height: keyHeight
                //visible: !keyboard.inSymView
                ////anchors.horizontalCenter: parent.horizontalCenter

                //CharacterKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "\u4E36"
                    //captionShifted: "4"
                //}
                //CharacterKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "\u4E5B"
                    //captionShifted: "5"
                //}
                //CharacterKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "?"
                    //captionShifted: "6"
                //}
                //FunctionKey {
                    //width: strokeKeyWidth
                    //height: keyHeight
                    //caption: "\u5220"
                    //key: Qt.Key_Delete
                //}
            //}
        //}
    //}
    Row {
        id: strokeRow1
        //property int strokeKeyWidth: (parent.width - backspace.width) / 5

        height: keyHeight
        visible: !keyboard.inSymView
        //anchors.horizontalCenter: parent.horizontalCenter

        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "！"
            captionShifted: "："
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "\u4E00"
            captionShifted: "1"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "\u4E28"
            captionShifted: "2"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "\u4E3F"
            captionShifted: "3"
        }

        BackspaceKey {
            id: backspace
            height: keyHeight
            width: strokeKeyWidth
        }
    }
    Row {
        id: strokeRow2
        //property int strokeKeyWidth: (parent.width - backspace.width) / 6

        height: keyHeight
        visible: !keyboard.inSymView
        //anchors.horizontalCenter: parent.horizontalCenter

        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "。"
            captionShifted: "."
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "\u4E36"
            captionShifted: "4"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "\u4E5B"
            captionShifted: "5"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "?"
            captionShifted: "6"
        }
        FunctionKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "\u5220"
            key: Qt.Key_Delete
        }
    }
    Row {
        id: strokeRow3
        //property int strokeKeyWidth: (parent.width - backspace.width) / 6

        height: keyHeight
        visible: !keyboard.inSymView
        anchors.horizontalCenter: parent.horizontalCenter

        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "？"
            captionShifted: "0"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "7"
            captionShifted: "7"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "8"
            captionShifted: "8"
        }
        CharacterKey {
            width: strokeKeyWidth
            height: keyHeight
            caption: "9"
            captionShifted: "9"
        }
        FunctionKey {
            width: strokeKeyWidth/2
            height: keyHeight
            caption: "←"
            key: Qt.Key_Left
        }
        FunctionKey {
            width: strokeKeyWidth/2
            height: keyHeight
            caption: "→"
            key: Qt.Key_Right
        }
    }

    KeyboardRow {
        SymbolKey {
            caption: keyboard.inSymView ? "笔画" : "符号" // stroke/symbols
        }

        ChineseContextAwareCommaKey {}
        //ShiftKey {}
        SpacebarKey {}
        //ChineseContextAwarePeriodKey {}
        ShiftKey {}
        EnterKey {}
    }
}
