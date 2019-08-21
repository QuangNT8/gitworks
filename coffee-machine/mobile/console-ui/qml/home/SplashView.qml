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
        Label
        {
            text: "BrainJuice Console"
            font.pixelSize: 32
            anchors.horizontalCenter: parent.horizontalCenter
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
        target: Console
        onLogged: timer.running = true
        onLoggedOut:
        {
            loading.visible = false
            busyIndication.visible = false
            popup.open()
        }
    }

    Popup
    {
        id: popup
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        implicitWidth: 250
        implicitHeight: 150
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose
        bottomInset: 0
        horizontalPadding: 0
        contentItem: Column
        {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Label
            {
                width: parent.width
                font.capitalization: Font.AllUppercase
                font.bold: true
                font.pixelSize: 13
                text: "warning"
                horizontalAlignment: "AlignHCenter"
            }

            Label
            {
                width: parent.width
                font.pixelSize: 13
                text: "Your account is logged on another device."
                wrapMode: Text.WordWrap
            }
            Label
            {
                width: parent.width
                font.pixelSize: 13
                text: "WARNING: If you cannot connect to the device, please turn off the device and try again."
                wrapMode: Text.WordWrap
            }
        }
    }
}
