import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import "layouts"
import xyz.birdzhang.stroke 1.0

MInputMethod {
    id: handler
    property bool strokeMode: true
    property bool showCandidates: false
    property string strokeSequence: ""
    property var candidates: []
    
    property alias keyboard: keyboard
    
    Rectangle {
        id: keyboard
        width: parent.width
        height: parent.height
        color: "#000000"
        opacity: 0.9
        
        property bool inSymView: false
        property bool pinyinMode: handler.strokeMode
        property int keyHeight: parent.height * 0.2
        
        zh_cn_souniaostroke {
            id: strokeLayout
            width: parent.width
            height: parent.height
            attributes: {
                inSymView: keyboard.inSymView
            }
        }
    }
    
    function handleKeyClick(key) {
        var handled = false
        
        if (key.key === Qt.Key_Backspace) {
            if (strokeSequence.length > 0) {
                strokeSequence = strokeSequence.substring(0, strokeSequence.length - 1)
                updateCandidates()
                handled = true
            }
        } else if (key.key === Qt.Key_Space) {
            if (candidates.length > 0) {
                commit(candidates[0])
                strokeSequence = ""
                updateCandidates()
                handled = true
            }
        } else if (key.key === Qt.Key_Return) {
            commit("\n")
            handled = true
        } else if (key.text === "符号") {
            keyboard.inSymView = !keyboard.inSymView
            handled = true
        } else if (key.text === "笔画") {
            keyboard.inSymView = false
            handled = true
        } else if (!keyboard.inSymView) {
            // 处理笔画输入
            var stroke = key.text
            if (stroke === "一" || stroke === "丨" || stroke === "丿" || stroke === "丶" || stroke === "乛" || stroke === "?") {
                strokeSequence += stroke
                updateCandidates()
                handled = true
            }
        } else {
            // 处理符号输入
            commit(key.text)
            handled = true
        }
        
        return handled
    }
    
    function updateCandidates() {
        if (strokeSequence.length > 0) {
            // 调用笔画识别库获取候选词
            candidates = strokeRecognition.recognize(strokeSequence)
            showCandidates = true
        } else {
            candidates = []
            showCandidates = false
        }
    }
    
    function commit(text) {
        MInputMethodQuick.sendCommit(text)
    }
    
    function clearPreedit() {
        MInputMethodQuick.sendPreedit("")
    }
    
    function selectCandidate(index) {
        if (index >= 0 && index < candidates.length) {
            commit(candidates[index])
            strokeSequence = ""
            updateCandidates()
        }
    }
    
    onActiveChanged: {
        if (active) {
            strokeSequence = ""
            candidates = []
            showCandidates = false
            keyboard.inSymView = false
        }
    }
    
    // 笔画识别组件
    StrokeRecognition {
        id: strokeRecognition
    }
}