import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import '../commons'

Rectangle
{
    anchors.fill: parent
    Column
    {
        anchors.centerIn: parent
        spacing: 5
//        Label
//        {
//            text: "Brain Juice Order"
//            font.pixelSize: 32
//            anchors.horizontalCenter: parent.horizontalCenter
//        }
        Item
        {
            width: 150
            height: width
            Image
            {
                anchors.fill: parent
                asynchronous: true
                antialiasing: true
                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(parent.width, parent.height)
                source: "qrc:/images/bj_logo.png"
            }
        }

        LoadingIndicator
        {
            id: busyIndication
            ringSize: 50
            nutSize: 10
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label
        {
            id: loading
            text: "Starting up ..."
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Timer
    {
        id: timer
        interval: 1000; running: false; repeat: false
        onTriggered: showView('qrc:/home/HomeView.qml')
    }

    Connections
    {
        target: Home
        onLogged: timer.running = true
    }
}
