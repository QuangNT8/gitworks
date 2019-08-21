import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

Dialog
{
    property alias message: txtMessage.text
    title: "Delete"
    x: (parent.width - width)/2
    y: (parent.height - height)/2
    width: parent.width - 20
    implicitHeight: 160
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    standardButtons: Dialog.Yes | Dialog.No

    contentItem:Label
    {
        id: txtMessage
        width: parent.width - 20
        height: 100
        font.pixelSize: 15
        wrapMode: "WordWrap"
    }
}

