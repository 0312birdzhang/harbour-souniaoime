import QtQuick 2.0
import Sailfish.Silica 1.0

Page{

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

            Label{
                text: "本程序大量参考（前端是直接抄的）mumudada的山寨谷歌，后端采用Qt5.11的pinyin输入法代码，特此声明。<br/>"
                       +"使用方式：到【设置】-【文本输入】中勾选【搜鸟输入法】，然后在输入的界面切换到本输入法即可，不需要重启。"
                       +"<br/>本程序只是测试一下开源的谷歌拼音输入法，并无其他意图，在旗鱼上更智能的输入法是不存在的~"
                wrapMode: Text.RichText
                font.pixelSize: Theme.fontSizeTiny
                color: Theme.primaryColor
                width: parent.width
                anchors{
                    left: parent.left
                    right: parent.right
                    rightMargin: Theme.paddingMedium
                    leftMargin: Theme.paddingMedium
                }

            }

            Item{
                width: 23.33
                height: 23.33
            }

            Image{
                source: "./notexist.jpg"
                width: parent.width
                height: width
                anchors.margins: Theme.paddingSmall
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
