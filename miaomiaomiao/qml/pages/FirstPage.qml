import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0

Page{

    ListModel{
        id: convertModel
    }

    ConfigurationGroup{
        id: config
        path: "/app/xyz.birdzhang.ime"
        property int pageSize: 20
        property int fetchSize: 15
        property bool traditional: false
        property string convertModel: "s2twp"
    }
    SilicaFlickable{
        anchors.fill: parent
        contentHeight: column.height
        Column{
            id: column
            width: parent.width
            spacing: Theme.paddingMedium
            PageHeader{
                title: "关于"
            }

            SectionHeader{
                text: "说明"
                font.pixelSize: Theme.fontSizeMedium
            }
            Label{
                text: "本程序大量参考木木大大的【山寨谷歌输入法】跟gexc大大的【拼音键盘】，后端采用谷歌拼音输入法代码，做到了既可以使用gexc大大的输入法键盘，又"+
                      "有木木大大输入法的词汇，并且无Xt9依赖可以安装在移植的机型上，特此声明。<br/>"
                      +"<br/>使用方式：到【设置】-【文本输入】中勾选【搜鸟输入法】，然后在输入的界面切换到本输入法即可，不需要重启。"
                      +"<br/><br/>本程序只是测试一下开源的谷歌拼音输入法，最终还是希望官方能够做到更好的中文输入😂"
                      +"<br/><br/>This is a pinyin input method for simple chinese user."
                wrapMode: Text.RichText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                width: parent.width
                anchors{
                    left: parent.left
                    right: parent.right
                    rightMargin: Theme.paddingMedium
                    leftMargin: Theme.paddingMedium
                }

            }

            SectionHeader{
                text: "作者"
                font.pixelSize: Theme.fontSizeMedium
            }

            Label{
                text: "旗鱼俱乐部 @0312birdzhang 出品"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                width: parent.width - Theme.paddingLarge
                horizontalAlignment: Text.AlignRight
            }
            Label{
                text: "图标由蝉曦出品"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                width: parent.width - Theme.paddingLarge
                horizontalAlignment: Text.AlignRight
            }
            Label{
                text: "简繁转换插件由topiasv支持"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                width: parent.width - Theme.paddingLarge
                horizontalAlignment: Text.AlignRight
            }

            Item {
                width: parent.width
                height: Theme.paddingMedium
            }

            SectionHeader{
                text: "自定义设置"
                font.pixelSize: Theme.fontSizeMedium
            }

            SectionHeader{
                text: "联想词数量"
                font.pixelSize: Theme.fontSizeMedium * 0.8
            }
            Slider {
                minimumValue: 5
                maximumValue: 20
                stepSize: 1
                value: config.fetchSize
                width: parent.width
                valueText:  value
                onValueChanged: {
                    config.fetchSize = value;
                }
            }

            SectionHeader{
                text: "候选词数量"
                font.pixelSize: Theme.fontSizeMedium * 0.8
            }

            Slider {
                minimumValue: 10
                maximumValue: 25
                stepSize: 1
                value: config.pageSize
                width: parent.width
                valueText: value
                onValueChanged: {
                    config.pageSize = value;
                }
            }
        }
    }

    Component.onCompleted: {
        var models = ([{
                           val:"s2t",
                           name:"简体到繁体"
                       },{
                           val:"s2tw",
                           name:"简体到台湾正体"
                       },{
                           val:"s2hk",
                           name:"简体到香港繁体"
                       },{
                           val:"s2twp",
                           name:"简体到台湾正体(常用)"
                       }]);
        for ( var i in models   ){
            convertModel.append({"label": models[i].name,
                                    "value": models[i].val
                                });
        }
    }
}
