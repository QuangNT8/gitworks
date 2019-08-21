import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import '../commons'

Rectangle
{
    id: networkVisual
    Layout.fillHeight: true
    Layout.fillWidth: true
    property var networkData
//    property var networkData: ListModel
//    {
//        ListElement
//        {
//            items: [ ListElement{min: 0}, ListElement{min: 0}]
//        }
//        ListElement
//        {
//            items: [ ListElement{min: 0}, ListElement{min: 0}, ListElement{min: 0}]
//        }
//        ListElement
//        {
//            items: [ ListElement{min: 0}, ListElement{min: 0}, ListElement{min: 0}, ListElement{min: 0}, ListElement{min: 0}]
//        }
//        ListElement
//        {
//            items: [ ListElement{min: 0}, ListElement{min: 0}, ListElement{min: 0}]
//        }
//        ListElement
//        {
//            items: [ ListElement{min: 0}]
//        }
//    }

    color: '#ccc'
    Label
    {
        anchors.centerIn: parent
        text: 'No neural network data'
        color: 'white'
        visible: networkData? false: true
        font.pixelSize: 18
    }

    RowLayout
    {
        anchors.centerIn: parent
        spacing: 15
        Repeater
        {
            id: layers
            model: networkData
            Item
            {
                Layout.preferredHeight: 350
                Layout.preferredWidth: 40
                //color: '#aaa'

                ColumnLayout
                {
                    anchors.centerIn: parent
                    spacing: 10
                    Repeater
                    {
                        model: items
                        Rectangle
                        {
                            Layout.preferredWidth: 26
                            Layout.preferredHeight: 26
                            radius: 13
                            color: '#aaa'
                            border.color: '#ddd'
                        }
                    }
                }
            }
        }
    }
}
