import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

//ToolBar
//{
//    height: 55
//    Material.primary: '#aaa'
//    signal saveClicked()
//    RowLayout
//    {
//        spacing: 5
//        anchors.centerIn: parent
//        Item
//        {
//            width: 48
//            height: width
//            Rectangle
//            {
//                anchors.fill: parent
//                anchors.margins: 9
//                radius: width/2
//                color: 'transparent'
//                border.color: '#eee'
//                Text
//                {
//                    anchors.centerIn: parent
//                    text: Flaticon.get('delete')
//                    font.family: 'flaticon'
//                    color: '#eee'
//                    font.pixelSize: 9
//                }
//            }
//            MouseArea
//            {
//                anchors.fill: parent
//                onClicked: back()
//            }
//        }

//        Item
//        {
//            width: 48
//            height: width
//            Rectangle
//            {
//                anchors.fill: parent
//                anchors.margins: 9
//                radius: width/2
//                color: 'transparent'
//                border.color: '#eee'
//                Text
//                {
//                    anchors.centerIn: parent
//                    text: Flaticon.get('tick')
//                    font.family: 'flaticon'
//                    color: '#eee'
//                    font.pixelSize: 10
//                }
//            }
//            MouseArea
//            {
//                anchors.fill: parent
//                onClicked: saveClicked()
//            }
//        }
//    }
//}


ToolBar
{
    height: 65
    Material.primary: '#aaa'
    signal saveClicked()
    RowLayout
    {
        spacing: 10
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        Button
        {
            Layout.fillWidth: true
            Layout.preferredWidth: 100
            text: 'Cancel'
            font.pixelSize: 15
            onClicked: back()
        }

        Button
        {
            Layout.fillWidth: true
            Layout.preferredWidth: 100
            text: 'Save'
            font.pixelSize: 15
            highlighted: true
            onClicked: saveClicked()
        }
    }
}
