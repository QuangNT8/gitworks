import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item
{
    id: loading
    anchors.fill: parent
    visible: false
    z: 1000

    Rectangle
    {
        anchors.fill: parent
        color: "#ccc"
        opacity: 0.5

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            onClicked:{}
            onEntered: {}
            onExited: {}
        }
    }

    BusyIndicator
    {
        anchors.centerIn: parent
        running: true
    }
}
