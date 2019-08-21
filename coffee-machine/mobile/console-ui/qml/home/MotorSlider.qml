import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.4

RangeSlider
{
    id: control
    background: Rectangle
    {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 4
        width: control.availableWidth
        height: implicitHeight
        radius: 2
        color: "#bdbebf"
        Rectangle
        {
            x: control.first.visualPosition * parent.width
            width: control.second.visualPosition * parent.width - x
            height: parent.height
            color: "#bdbebf"
            radius: 2
        }
    }
    first.handle: Rectangle
    {
        x: control.leftPadding + parent.first.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 50
        implicitHeight: 50
        radius: 25
        color: parent.first.pressed ? "#F44336" : "#E91E63"
        border.color: "#bdbebf"
        Image
        {
            anchors.fill: parent
            anchors.margins: 5
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            sourceSize: Qt.size(parent.width, parent.height)
            source: 'qrc:/images/vending-paper-cup.png'
        }
        Label
        {
            id: txtFirstText
            anchors.centerIn: parent
            font.pixelSize: 13
            font.bold: first.pressed
            color: control.enabled ? 'black' : '#666'
            text: 'L'
            z: 10
        }
    }
    second.handle: Rectangle
    {
        x: control.leftPadding + parent.second.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 50
        implicitHeight: 50
        radius: 25
        color: parent.second.pressed ? "#F44336" : "#E91E63"
        border.color: "#bdbebf"
        Image
        {
            anchors.fill: parent
            anchors.margins: 5
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            sourceSize: Qt.size(parent.width, parent.height)
            source: 'qrc:/images/vending-paper-cup.png'
        }
        Label
        {
            id: txtSecondText
            anchors.centerIn: parent
            font.pixelSize: 13
            font.bold: second.pressed
            color: control.enabled ? 'black' : '#666'
            text: 'R'
            z: 10
        }
    }
}
