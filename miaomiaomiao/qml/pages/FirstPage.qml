import QtQuick 2.0
import Sailfish.Silica 1.0

Page{

    SilicaFlickable{
        anchors.fill: parent
        contentHeight: column.height
        Column{
            id: column
            width: parent.width
            PageHeader{
                title: "Test"
            }

            TextField{
                placeholderText: "Enter Username"
                label: "Username"
                focus: true
                width: parent.width
            }
        }
    }
}
