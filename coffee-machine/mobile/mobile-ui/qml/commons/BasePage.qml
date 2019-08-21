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
    property alias headerTitle: txtTitle.text
    property alias rightIcon: btRight.text
    property alias backgroundColor: recBackground.color

    property bool isHomePage: false
    property bool isBack: true

    Material.primary: Material.Brown
    Material.theme: Material.Light
    background: Rectangle
    {
        id: recBackground
        color:'#eee'
    }

    header:ColumnLayout
    {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        width: parent.width
        spacing: 0
        z:100

        Rectangle
        {
            //FIXME:
            id: appErrorBar
            width: parent.width
            height: 30
            visible: false
            color: '#FF5722'

            Label
            {
                id: txtAppErrorMessage
                anchors.centerIn: parent
                width: parent.width
                leftPadding: 20
                rightPadding: 20
                font.bold: true
                color: '#fff'
                elide: "ElideRight"
                horizontalAlignment: Text.AlignHCenter
            }
        }

        ToolBar
        {
            id: toolBar
            Layout.fillWidth: true
            visible: !isHomePage
            opacity: 0.75
            layer.enabled: true
            Rectangle
            {
                anchors.fill: parent
                opacity: 0.9
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "black" }
                    GradientStop { position: 0.43; color: "grey" }
                    GradientStop { position: 1.0; color: "white" }
                }
            }
            RowLayout
            {
                anchors.fill: parent
                spacing: 0
                ToolButton
                {
                    text: Flaticon.get('back')
                    font.family: 'Flaticon'
                    font.pixelSize: 13
                    onClicked:
                    {
                        if (isBack) back()
                        else leftClicked()
                    }
                }
                Label
                {
                    id: txtTitle
                    Layout.fillWidth: true
                    font.bold: true
                    font.pixelSize: 18
                    elide: "ElideRight"
                    horizontalAlignment: Qt.AlignHCenter
                }

                ToolButton
                {
                    id: btRight
                    text: ''
                    font.family: 'Flaticon'
                    font.pixelSize: 13
                    onClicked: rightClicked()
                    enabled: text.length > 0
                }
            }
        }
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

//    function showAppError(message)
//    {
//        txtAppErrorMessage.text = message
//        appErrorBar.visible = true;
//    }
//    function hideAppError(){appErrorBar.visible = false;}

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
