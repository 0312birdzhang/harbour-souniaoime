import QtQuick 2.0
import ".."

KeyboardLayout {
    type: "china_stroke"
    inputMode: "stroke"
    property bool pinyinMode: true
    
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


    Row {
        id: strokeRow
        property int strokeKeyWidth: (parent.width - backspace.width) / 6

        height: keyHeight
        visible: !keyboard.inSymView
        anchors.horizontalCenter: parent.horizontalCenter

        CharacterKey {
            width: strokeRow.strokeKeyWidth
            height: keyHeight
            caption: "\u4E00"
            captionShifted: "\u4E00"
        }
        CharacterKey {
            width: strokeRow.strokeKeyWidth
            height: keyHeight
            caption: "\u4E28"
            captionShifted: "\u4E28"
        }
        CharacterKey {
            width: strokeRow.strokeKeyWidth
            height: keyHeight
            caption: "\u4E3F"
            captionShifted: "\u4E3F"
        }
        CharacterKey {
            width: strokeRow.strokeKeyWidth
            height: keyHeight
            caption: "\u4E36"
            captionShifted: "\u4E36"
        }
        CharacterKey {
            width: strokeRow.strokeKeyWidth
            height: keyHeight
            caption: "\u4E5B"
            captionShifted: "\u4E5B"
        }
        CharacterKey {
            width: strokeRow.strokeKeyWidth
            height: keyHeight
            caption: "?"
            captionShifted: "?"
        }

        BackspaceKey {
            id: backspace
            height: keyHeight
        }
    }

    KeyboardRow {
        SymbolKey {
            caption: keyboard.inSymView ? "笔画" : "符号" // stroke/symbols
        }

        ChineseContextAwareCommaKey {}
        SpacebarKey {}
        ChineseContextAwarePeriodKey {}
        EnterKey {}
    }
}
