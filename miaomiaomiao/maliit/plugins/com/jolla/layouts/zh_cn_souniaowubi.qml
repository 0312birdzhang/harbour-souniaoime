import QtQuick 2.0
import ".."

KeyboardLayout {
    id: wubiLayout
    type: "china_wubi"
    inputMode: "simplified"
    property bool pinyinMode: true
    splitSupported: true

    KeyboardRow {
        CharacterKey { caption: "q"; captionShifted: "Q" }
        CharacterKey { caption: "w"; captionShifted: "W" }
        CharacterKey { caption: "e"; captionShifted: "E" }
        CharacterKey { caption: "r"; captionShifted: "R" }
        CharacterKey { caption: "t"; captionShifted: "T" }
        CharacterKey { caption: "y"; captionShifted: "Y" }
        CharacterKey { caption: "u"; captionShifted: "U" }
        CharacterKey { caption: "i"; captionShifted: "I" }
        CharacterKey { caption: "o"; captionShifted: "O" }
        CharacterKey { caption: "p"; captionShifted: "P" }
    }

    KeyboardRow {
        splitIndex: 5

        CharacterKey { caption: "a"; captionShifted: "A" }
        CharacterKey { caption: "s"; captionShifted: "S" }
        CharacterKey { caption: "d"; captionShifted: "D" }
        CharacterKey { caption: "f"; captionShifted: "F" }
        CharacterKey { caption: "g"; captionShifted: "G" }
        CharacterKey { caption: "h"; captionShifted: "H" }
        CharacterKey { caption: "j"; captionShifted: "J" }
        CharacterKey { caption: "k"; captionShifted: "K" }
        CharacterKey { caption: "l"; captionShifted: "L" }
    }

    KeyboardRow {
        ShiftKey {}

        CharacterKey { caption: "z"; captionShifted: "Z" }
        CharacterKey { caption: "x"; captionShifted: "X" }
        CharacterKey { caption: "c"; captionShifted: "C" }
        CharacterKey { caption: "v"; captionShifted: "V" }
        CharacterKey { caption: "b"; captionShifted: "B" }
        CharacterKey { caption: "n"; captionShifted: "N" }
        CharacterKey { caption: "m"; captionShifted: "M" }

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
