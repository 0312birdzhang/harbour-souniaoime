/*
 * Copyright (C) 2012-2013 Jolla ltd and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: Pekka Vuorela <pekka.vuorela@jollamobile.com>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Jolla Ltd nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import com.meego.maliitquick 1.0
import com.jolla.keyboard 1.0
import ".."

KeyboardLayout {
    id: pinyinLayout
    type: "china_pinyin"
    inputMode: "simplified"
    splitSupported: true
    property bool pinyinMode: MInputMethodQuick.contentType !== Maliit.UrlContentType && MInputMethodQuick.contentType !== Maliit.EmailContentType

    KeyboardRow {
        CharacterKey { caption: "q"; captionShifted: "Q"; symView: "1"; symView2: pinyinLayout.pinyinMode ? "！" : "!" }
        CharacterKey { caption: "w"; captionShifted: "W"; symView: "2"; symView2: pinyinLayout.pinyinMode ? "＠" : "@" }
        CharacterKey { caption: "e"; captionShifted: "E"; symView: "3"; symView2: pinyinLayout.pinyinMode ? "＃" : "#" }
        CharacterKey { caption: "r"; captionShifted: "R"; symView: "4"; symView2: pinyinLayout.pinyinMode ? "￥" : "$" }
        CharacterKey { caption: "t"; captionShifted: "T"; symView: "5"; symView2: pinyinLayout.pinyinMode ? "％" : "%" }
        CharacterKey { caption: "y"; captionShifted: "Y"; symView: "6"; symView2: pinyinLayout.pinyinMode ? "…" : "^" }
        CharacterKey { caption: "u"; captionShifted: "U"; symView: "7"; symView2: pinyinLayout.pinyinMode ? "＆" : "&"}
        CharacterKey { caption: "i"; captionShifted: "I"; symView: "8"; symView2: pinyinLayout.pinyinMode ? "×" : "*" }
        CharacterKey { caption: "o"; captionShifted: "O"; symView: "9"; symView2: pinyinLayout.pinyinMode ? "（" : "(" }
        CharacterKey { caption: "p"; captionShifted: "P"; symView: "0"; symView2: pinyinLayout.pinyinMode ? "）" : ")"}
    }

    KeyboardRow {
        splitIndex: 5
        CharacterKey { caption: "a"; captionShifted: "A"; symView: pinyinLayout.pinyinMode ? "【" : "["; symView2: pinyinLayout.pinyinMode ? "｛" : "{"}
        CharacterKey { caption: "s"; captionShifted: "S"; symView: pinyinLayout.pinyinMode ? "】" : "]"; symView2: pinyinLayout.pinyinMode ? "｝" : "}" }
        CharacterKey { caption: "d"; captionShifted: "D"; symView: pinyinLayout.pinyinMode ? "、" : "\\"; symView2: pinyinLayout.pinyinMode ? "｜" : "|"}
        CharacterKey {
            showPopper: keyboard.inSymView ? false : true
            caption: keyboard.inSymView ? "◀" : "f";
            captionShifted: keyboard.inSymView ? "◀" : "F";
            text: keyboard.inSymView ? "" : keyboard.isShifted ? "F" : "f"
            key: keyboard.inSymView ? Qt.Key_Home : Qt.Key_F;
            repeat: keyboard.inSymView? true : false;
            Rectangle{
                visible: keyboard.inSymView
                anchors.fill: parent
                anchors.margins: Theme.paddingMedium
                opacity: 0.05
                radius: geometry.keyRadius
            }
        }
        CharacterKey {
            showPopper: keyboard.inSymView ? false : true
            caption: keyboard.inSymView ? "△" : "g";
            captionShifted: keyboard.inSymView ? "△" : "G";
            text: keyboard.inSymView ? "" : keyboard.isShifted ? "G" : "g"
            key: keyboard.inSymView ? Qt.Key_Up : Qt.Key_G;
            repeat: keyboard.inSymView? true : false;
            Rectangle{
                visible: keyboard.inSymView
                anchors.fill: parent
                anchors.margins: Theme.paddingMedium
                opacity: 0.05
                radius: geometry.keyRadius
            }
        }
        CharacterKey {
            showPopper: keyboard.inSymView ? false : true
            caption: keyboard.inSymView ? "▶" : "h";
            captionShifted: keyboard.inSymView ? "▶" : "H";
            text: keyboard.inSymView ? "" : keyboard.isShifted ? "H" : "h"
            key: keyboard.inSymView ? Qt.Key_End : Qt.Key_H;
            repeat: keyboard.inSymView? true : false;
            Rectangle{
                visible: keyboard.inSymView
                anchors.fill: parent
                anchors.margins: Theme.paddingMedium
                opacity: 0.05
                radius: geometry.keyRadius
            }
        }
        CharacterKey { caption: "j"; captionShifted: "J"; symView: pinyinLayout.pinyinMode ? "－" : "-"; symView2: pinyinLayout.pinyinMode ? "—" : "_"}
        CharacterKey { caption: "k"; captionShifted: "K"; symView: pinyinLayout.pinyinMode ? "＝" : "="; symView2: pinyinLayout.pinyinMode ? "＋" : "+"}
        CharacterKey { caption: "l"; captionShifted: "L"; symView: pinyinLayout.pinyinMode ? "｀" : "`"; symView2: pinyinLayout.pinyinMode ? "～" : "~" }
    }

    KeyboardRow {
        ShiftKey {
            enabled: !pinyinLayout.pinyinMode
        }

        CharacterKey { caption: "z"; captionShifted: "Z"; symView: pinyinLayout.pinyinMode ? "／" : "/"; symView2: pinyinLayout.pinyinMode ? "？" : "?"}
        CharacterKey { caption: "x"; captionShifted: "X"; symView: pinyinLayout.pinyinMode ? "‘" : "€"; symView2: pinyinLayout.pinyinMode ? "“" : "£"}
        CharacterKey {
            showPopper: keyboard.inSymView ? false : true
            caption: keyboard.inSymView ? "◁" : "c";
            captionShifted: keyboard.inSymView ? "◁" : "C";
            text: keyboard.inSymView ? "" : keyboard.isShifted ? "C" : "c"
            key: keyboard.inSymView ? Qt.Key_Left : Qt.Key_C;
            repeat: keyboard.inSymView? true : false;
            Rectangle{
                visible: keyboard.inSymView
                anchors.fill: parent
                anchors.margins: Theme.paddingMedium
                opacity: 0.05
                radius: geometry.keyRadius
            }
        }

        KeySeparator {
            visible: (separator === SeparatorState.AutomaticSeparator && implicitSeparator)
        }
        CharacterKey {
            showPopper: keyboard.inSymView ? false : true
            caption: keyboard.inSymView ? "▽" : "v";
            captionShifted: keyboard.inSymView ? "▽" : "V";
            text: keyboard.inSymView ? "" : keyboard.isShifted ? "V" : "v"
            key: keyboard.inSymView ? Qt.Key_Down : Qt.Key_V;
            repeat: keyboard.inSymView? true : false;
            Rectangle{
                visible: keyboard.inSymView
                anchors.fill: parent
                anchors.margins: Theme.paddingMedium
                opacity: 0.05
                radius: geometry.keyRadius
            }
        }
        CharacterKey {
            showPopper: keyboard.inSymView ? false : true
            caption: keyboard.inSymView ? "▷" : "b";
            captionShifted: keyboard.inSymView ? "▷" : "B";
            text: keyboard.inSymView ? "" : keyboard.isShifted ? "B" : "b"
            key: keyboard.inSymView ? Qt.Key_Right : Qt.Key_B;
            repeat: keyboard.inSymView? true : false;
            Rectangle{
                visible: keyboard.inSymView
                anchors.fill: parent
                anchors.margins: Theme.paddingMedium
                opacity: 0.05
                radius: geometry.keyRadius
            }
        }
        CharacterKey { caption: "n"; captionShifted: "N"; symView: pinyinLayout.pinyinMode ? "；" : ";"; symView2 : pinyinLayout.pinyinMode ? "：" : ":" }
        CharacterKey { caption: "m"; captionShifted: "M"; symView: pinyinLayout.pinyinMode ? "’" : "\'"; symView2: pinyinLayout.pinyinMode ? "”" : "\"" }

        BackspaceKey {}
    }


    KeyboardRow {
        splitIndex: 4
        SymbolKey {
            caption: keyboard.inSymView ? ( pinyinLayout.pinyinMode ? "拼音" : "En") : "符号" // symbols/pinyin
            //implicitWidth: functionKeyWidth * 0.75
        }
        /*FunctionKey {
                //icon.source: "image://theme/icon-l-left" + (pressed ? ("?" + Theme.highlightColor) : "")
        caption: "←"
                repeat: true
                key: Qt.Key_C
                implicitWidth: functionKeyWidth * 0.35
        }*/


        CharacterKey {
            caption: pinyinLayout.pinyinMode? "，" : MInputMethodQuick.contentType === Maliit.UrlContentType
                                              ? "/"
                                              : MInputMethodQuick.contentType === Maliit.EmailContentType
                                                ? "@"
                                                : ","
            captionShifted: pinyinLayout.pinyinMode? "，" : MInputMethodQuick.contentType === Maliit.UrlContentType
                                                     ? "/"
                                                     : MInputMethodQuick.contentType === Maliit.EmailContentType
                                                       ? "@"
                                                       : ","
            symView: ","
            symView2: pinyinLayout.pinyinMode? "《" : "<"
            implicitWidth: punctuationKeyWidth * 0.7
            fixedWidth: !splitActive
            separator: SeparatorState.HiddenSeparator

        }


        SpacebarKey {
            //implicitWidth: parent.width - functionKeyWidth * 2.2 - punctuationKeyWidth * 1.4
            fixedWidth: !splitActive
        }
        SpacebarKey {
            active: splitActive
            languageLabel: ""
        }

        CharacterKey {
            caption: pinyinLayout.pinyinMode ? "。" : MInputMethodQuick.contentType === Maliit.UrlContentType
                                               ? "."
                                               : MInputMethodQuick.contentType === Maliit.EmailContentType
                                                 ? "."
                                                 : "."
            captionShifted: pinyinLayout.pinyinMode ? "。" : MInputMethodQuick.contentType === Maliit.UrlContentType
                                                      ? "."
                                                      : MInputMethodQuick.contentType === Maliit.EmailContentType
                                                        ? "."
                                                        : "."
            symView: "."
            symView2: pinyinLayout.pinyinMode? "》" : ">"
            implicitWidth: punctuationKeyWidth * 0.7
            width: punctuationKeyWidth * 0.7
            fixedWidth: !splitActive
            separator: SeparatorState.HiddenSeparator

        }
        /*FunctionKey {
                //icon.source: "image://theme/icon-l-right" + (pressed ? ("?" + Theme.highlightColor) : "")
        caption: "→"
                repeat: true
                key: Qt.Key_Right
                implicitWidth: functionKeyWidth * 0.35
        }*/

        EnterKey {
            //implicitWidth: functionKeyWidth * 0.75
        }
    }
}
