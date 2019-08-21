import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0

Page
{
    id: page
    signal rightClicked
    signal leftClicked
    property bool isHomePage: false
    property bool isBack: true

    Material.primary: Material.Brown
    background: Rectangle
    {
        id: recBackground
        color:'#fff'
    }

    Rectangle
    {
        id: pageErrorBar
        anchors.top: parent.top
        width: parent.width
        height: 30
        visible: false
        color: '#FF9800'
        opacity: 0.9
        z:100

        Timer
        {
            id: errorTimer
            interval: 500; running: true; repeat: false
            onTriggered: pageErrorBar.visible = false;
        }

        RowLayout
        {
            anchors.fill: parent
            Label
            {
                id: txtPageErrorMessage
                Layout.fillWidth: true
                leftPadding: 40
                color: 'white'
                elide: "ElideRight"
                horizontalAlignment: Text.AlignHCenter
            }
            ToolButton
            {
                text: Flaticon.get('delete')
                font.family: 'Flaticon'
                font.pixelSize: 13
                onClicked: hidePageError()
                Layout.preferredHeight: 30
                Layout.preferredWidth: 40
                Material.theme: Material.Dark
            }
        }
    }

    function showAppError(message)
    {
        txtAppErrorMessage.text = message
        appErrorBar.visible = true;
    }
    function hideAppError(){appErrorBar.visible = false;}

    function showPageError(message, timeout)
    {
        txtPageErrorMessage.text = message
        pageErrorBar.visible = true;
        if (timeout>0)
        {
            errorTimer.interval = timeout
            errorTimer.start()
        }
    }
    function hidePageError(){pageErrorBar.visible = false;}
}
