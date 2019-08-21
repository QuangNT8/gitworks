import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "commons"

Window
{
    visible: true
    title: qsTr("Mobile")

    Component.onCompleted:
    {
        showPage('qrc:/home/SplashView.qml')
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

    onClosing:
    {
        if (loading.visible)
        {
            close.accepted = false
            return
        }

        if(pageLoader.depth > 1)
        {
            close.accepted = false
            pageLoader.pop();
        }
        else close.accepted = true;
    }

    function showAppError(message, timeout)
    {
        txtError.text = message
        if (timeout > 0)
        {
            errorTimer.interval = timeout
            errorTimer.start()
        }

        errorMessage.visible = true
    }
    function hideAppError()
    {
        errorMessage.visible = false
    }

    function showPage(url, param)
    {
        var item = pageLoader.push(url, param)
    }
    function back()
    {
        pageLoader.pop()
    }

    function home()
    {
        pageLoader.pop(null)
    }

    function showView(item)
    {
        pageLoader.clear()
        pageLoader.push(item)
    }

    function setBusy(busy)
    {
        pageLoader.enabled = !busy
        busyArea.visible = busy
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
