import QtQuick 2.0
import ".."

KeyboardLayout {
    id: wubiLayout
    type: "china_wubi"
    inputMode: "simplified"
    property bool pinyinMode: true
    splitSupported: true

    KeyboardRow {
        CharacterKey { caption: "q"; captionShifted: "Q"; symView: "1"; symView2: "@" }
        CharacterKey { caption: "w"; captionShifted: "W"; symView: "2"; symView2: "/" }
        CharacterKey { caption: "e"; captionShifted: "E"; symView: "3"; symView2: "\\" }
        CharacterKey { caption: "r"; captionShifted: "R"; symView: "4"; symView2: "~" }
        CharacterKey { caption: "t"; captionShifted: "T"; symView: "5"; symView2: "^" }
        CharacterKey { caption: "y"; captionShifted: "Y"; symView: "6"; symView2: "_" }
        CharacterKey { caption: "u"; captionShifted: "U"; symView: "7"; symView2: "¥" }
        CharacterKey { caption: "i"; captionShifted: "I"; symView: "8"; symView2: "€" }
        CharacterKey { caption: "o"; captionShifted: "O"; symView: "9"; symView2: "$" }
        CharacterKey { caption: "p"; captionShifted: "P"; symView: "0"; symView2: "£" }
    }

    KeyboardRow {
        splitIndex: 5

        CharacterKey { caption: "a"; captionShifted: "A"; symView: "*"; symView2: "§" }
        CharacterKey { caption: "s"; captionShifted: "S"; symView: "#"; symView2: "=" }
        CharacterKey { caption: "d"; captionShifted: "D"; symView: "+"; symView2: "〈" }
        CharacterKey { caption: "f"; captionShifted: "F"; symView: "-"; symView2: "〉" }
        CharacterKey { caption: "g"; captionShifted: "G"; symView: "（"; symView2: "(" }
        CharacterKey { caption: "h"; captionShifted: "H"; symView: "）"; symView2: ")" }
        CharacterKey { caption: "j"; captionShifted: "J"; symView: "—"; symView2: "《" }
        CharacterKey { caption: "k"; captionShifted: "K"; symView: "…"; symView2: "》" }
        CharacterKey { caption: "l"; captionShifted: "L"; symView: "%"; symView2: "&" }
    }

    KeyboardRow {
        ShiftKey {}

        CharacterKey { caption: "z"; captionShifted: "Z"; symView: "“"; symView2: "'" }
        CharacterKey { caption: "x"; captionShifted: "X"; symView: "”"; symView2: "\"" }
        CharacterKey { caption: "c"; captionShifted: "C"; symView: "；"; symView2: ";" }
        CharacterKey { caption: "v"; captionShifted: "V"; symView: "："; symView2: ":" }
        CharacterKey { caption: "b"; captionShifted: "B"; symView: "、"; symView2: "·" }
        CharacterKey { caption: "n"; captionShifted: "N"; symView: "！"; symView2: "!" }
        CharacterKey { caption: "m"; captionShifted: "M"; symView: "？"; symView2: "?" }

        BackspaceKey {}
    }

    KeyboardRow {
        splitIndex: 3
        SymbolKey {
            caption: keyboard.inSymView ? "五笔" : "符号"
        }

        ChineseContextAwareCommaKey {}

        SpacebarKey {}
        SpacebarKey {
            active: splitActive
            languageLabel: ""
        }

        ChineseContextAwarePeriodKey {}

        EnterKey {}
    }
}
