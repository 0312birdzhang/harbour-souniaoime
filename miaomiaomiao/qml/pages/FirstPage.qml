import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0

Page{


    ConfigurationGroup{
        id: config
        path: "/app/xyz.birdzhang.ime"
        property int pageSize: 20
        property int fetchSize: 15
        property string wubiVersion: "wubi86"
    }
    SilicaFlickable{
        anchors.fill: parent
        contentHeight: column.height
        PullDownMenu {
            MenuItem {
                text: "关于"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: "自定义谷歌拼音词典"
                onClicked: pageStack.push(Qt.resolvedUrl("UserDictionaryPage.qml"))
            }
        }
        Column{
            id: column
            width: parent.width
            spacing: Theme.paddingMedium
            PageHeader{
                title: "设置"
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

            SectionHeader{
                text: "五笔版本"
                font.pixelSize: Theme.fontSizeMedium * 0.8
            }

            ComboBox {
                id: wubiVersionCombo
                width: parent.width
                label: "版本"
                description: "切换后立即生效，无需重启"
                currentIndex: {
                    switch (config.wubiVersion) {
                    case "wubi98":  return 1
                    case "wubixsj": return 2
                    default:        return 0
                    }
                }

                menu: ContextMenu {
                    MenuItem {
                        text: "86版"
                        onClicked: config.wubiVersion = "wubi86"
                    }
                    MenuItem {
                        text: "98版"
                        onClicked: config.wubiVersion = "wubi98"
                    }
                    MenuItem {
                        text: "新世纪版"
                        onClicked: config.wubiVersion = "wubixsj"
                    }
                }
            }
        }
    }

}
