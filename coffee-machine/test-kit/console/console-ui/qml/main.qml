import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "commons"

Window
{
    visible: true
    title: qsTr("Kit Console")

    Component.onCompleted:
    {
        showView('qrc:/home/SplashView.qml')
    }

    FontLoader
    {
        source: 'qrc:/font/Flaticon.ttf'
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0
        Rectangle
        {
            id: errorMessage
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: '#E91E63'
            Label
            {
                id: txtError
                anchors.centerIn: parent
                text: 'Connecting to internet...'
                font.pixelSize: 14
                font.bold: true
                color: '#eee'
            }
            visible: false

            Timer
            {
                id: errorTimer
                interval: 500; running: false; repeat: false
                onTriggered: errorMessage.visible = false;
            }
        }

        StackView
        {
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: pageLoader
        }
    }

    function showView(item)
    {
        pageLoader.clear()
        pageLoader.push(item)
    }

    Loading
    {
        id: loading
    }

    function showLoading()
    {
        loading.visible = true
    }

    function hideLoading()
    {
        loading.visible = false
    }
}
