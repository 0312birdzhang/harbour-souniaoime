import QtQuick 2.0
import Sailfish.Silica 1.0
import xyz.birdzhang.ime 1.0

Page {
    id: page
    property string originalPhrase: ""
    property string originalPinyin: ""
    property bool editing: originalPhrase.length > 0
    property int nextOffset: 0
    property int totalRawEntries: 0
    property bool hasMore: true
    property bool loading: false
    property int pageSize: 100
    property bool initialized: false
    property string phraseInput: ""
    property string pinyinInput: ""
    property string statusMessage: ""

    function initializePage() {
        if (initialized)
            return
        if (pinyin.init()) {
            initialized = true
            resetEntries("")
        } else {
            statusMessage = "词典初始化失败"
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active)
            initializeTimer.start()
    }
    Component.onCompleted: initializeTimer.start()

    Timer {
        id: initializeTimer
        interval: 0
        repeat: false
        onTriggered: initializePage()
    }

    function resetEntries(message) {
        entryModel.clear()
        nextOffset = 0
        totalRawEntries = 0
        hasMore = true
        statusMessage = message || ""
        loadNextPage()
    }

    function loadNextPage() {
        if (loading || !hasMore)
            return
        loading = true
        var pageRows = pinyin.userDictionaryEntryPage(nextOffset, pageSize)
        if (pageRows.length > 0 && pageRows[0].indexOf("@next:") === 0) {
            var metadata = pageRows[0].split(":")
            nextOffset = parseInt(metadata[1])
            totalRawEntries = parseInt(metadata[2])
            for (var i = 1; i < pageRows.length; ++i) {
                var separator = pageRows[i].indexOf("\t")
                if (separator > 0) {
                    entryModel.append({
                        "phraseText": pageRows[i].substring(0, separator),
                        "pinyinText": pageRows[i].substring(separator + 1).toLowerCase()
                    })
                }
            }
            hasMore = nextOffset < totalRawEntries
        } else {
            hasMore = false
            statusMessage = "读取用户词典失败"
        }
        loading = false
    }

    function beginEdit(phrase, spelling) {
        originalPhrase = phrase
        originalPinyin = spelling
        phraseInput = phrase
        pinyinInput = spelling.toLowerCase()
        list.positionViewAtBeginning()
    }

    function cancelEdit() {
        originalPhrase = ""
        originalPinyin = ""
        phraseInput = ""
        pinyinInput = ""
    }

    function saveEntry() {
        var wasEditing = editing
        var error = wasEditing
                ? pinyin.updateUserPhrase(originalPhrase, originalPinyin,
                                          phraseInput, pinyinInput)
                : pinyin.addUserPhrase(phraseInput, pinyinInput)
        if (error === "") {
            cancelEdit()
            resetEntries(wasEditing ? "修改成功，已写入用户词典"
                                    : "添加成功，已写入用户词典")
        } else {
            statusMessage = error
        }
    }

    ListModel { id: entryModel }

    SilicaListView {
        id: list
        anchors.fill: parent
        model: entryModel
        onAtYEndChanged: {
            if (atYEnd)
                loadNextPage()
        }

        PullDownMenu {
            MenuItem {
                text: "清空用户词典"
                onClicked: remorse.execute("正在清空用户词典", function() {
                    if (pinyin.resetUserDictionary()) {
                        page.cancelEdit()
                        page.resetEntries("用户词典已清空")
                    } else {
                        page.statusMessage = "清空失败，请检查文件权限"
                    }
                })
            }
        }

        RemorsePopup { id: remorse }

        header: Column {
            width: list.width
            spacing: Theme.paddingMedium

            PageHeader { title: editing ? "修改用户词条" : "自定义用户词典" }

            Label {
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: editing ? "正在修改：“" + originalPhrase + "”。"
                              : "添加 2—8 个汉字的词组；点击现有词条可查看和修改。"
            }

            TextField {
                id: phraseField
                width: parent.width
                text: page.phraseInput
                onTextChanged: page.phraseInput = text
                label: "词组"
                placeholderText: "搜鸟输入法"
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: pinyinField.focus = true
            }

            TextField {
                id: pinyinField
                width: parent.width
                text: page.pinyinInput
                onTextChanged: page.pinyinInput = text
                label: "完整拼音"
                placeholderText: "sou niao shu ru fa"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
                EnterKey.text: editing ? "保存" : "添加"
                EnterKey.onClicked: page.saveEntry()
            }

            Button {
                id: saveButton
                anchors.horizontalCenter: parent.horizontalCenter
                text: editing ? "保存修改" : "添加词条"
                enabled: page.phraseInput.length > 0 && page.pinyinInput.length > 0
                onClicked: page.saveEntry()
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "取消修改"
                visible: editing
                onClicked: page.cancelEdit()
            }

            Label {
                id: statusLabel
                text: page.statusMessage
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                visible: text.length > 0
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }

            SectionHeader {
                text: hasMore ? "现有词条（已加载 " + entryModel.count + "）"
                              : "现有词条（" + entryModel.count + "）"
            }

            Label {
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                visible: !loading && entryModel.count === 0
                text: "暂无用户词条"
                color: Theme.secondaryColor
                horizontalAlignment: Text.AlignHCenter
            }
        }

        delegate: ListItem {
            id: entryItem
            property string entryPhrase: phraseText
            property string entryPinyin: pinyinText
            width: list.width
            contentHeight: Theme.itemSizeMedium
            onClicked: page.beginEdit(entryPhrase, entryPinyin)

            Label {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                text: entryItem.entryPhrase + "    " + entryItem.entryPinyin
                color: entryItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                truncationMode: TruncationMode.Fade
            }

            menu: ContextMenu {
                MenuItem {
                    text: "删除"
                    onClicked: remorse.execute("删除“" + entryItem.entryPhrase + "”", function() {
                        var error = pinyin.removeUserPhrase(entryItem.entryPhrase,
                                                            entryItem.entryPinyin)
                        if (error === "") {
                            if (originalPhrase === entryItem.entryPhrase &&
                                    originalPinyin === entryItem.entryPinyin)
                                page.cancelEdit()
                            page.resetEntries("删除成功")
                        } else {
                            page.statusMessage = error
                        }
                    })
                }
            }
        }

        footer: Item {
            width: list.width
            height: hasMore ? Theme.itemSizeSmall : Theme.paddingLarge

            Label {
                anchors.centerIn: parent
                visible: hasMore
                text: loading ? "正在加载…" : "继续上拉加载"
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
        }
    }

    QmlPinyin {
        id: pinyin
        Component.onDestruction: pinyin.flushCache()
    }
}
