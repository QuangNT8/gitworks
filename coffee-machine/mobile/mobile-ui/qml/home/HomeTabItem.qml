import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

TabButton
{
    id: tabItem
    property alias symbol: txtIcon.text
    property alias title: txtTitle.text
    property int notif: 0
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    ColumnLayout
    {
        spacing: 0
        anchors.centerIn: parent
        Label
        {
            id: txtIcon
            Layout.alignment: Qt.AlignHCenter
            font.family: 'Flaticon'
            font.pixelSize: 25
            color: tabItem.checked ? Material.color(Material.Brown): '#666'
            font.bold: tabItem.checked ? true: false

            Rectangle
            {
                visible: notif>0
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: -10
                anchors.topMargin: -6
                width: 20
                height: 20
                color: "#FF5722"
                radius: 10
                border.width: 1
                border.color: '#eee'
                Text
                {
                    anchors.centerIn: parent
                    text: notif
                    color: '#fff'
                    font.bold: true
                    font.pixelSize: 11
                }
            }
        }
        Label
        {
            id: txtTitle
            Layout.alignment: Qt.AlignHCenter
            text: 'Automation'
            color: tabItem.checked ? Material.color(Material.Brown): '#666'
        }
    }
}
