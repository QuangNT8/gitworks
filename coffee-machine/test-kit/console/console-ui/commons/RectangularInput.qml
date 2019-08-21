import QtQuick 2.12


Rectangle
{
    property alias text: textInput.text
    property alias maximumLength: textInput.maximumLength
    property alias color: textInput.color
    property alias validator: textInput.validator
    property alias pixelSize: textInput.font.pixelSize
    id: control
    width: parent.width
    height: 35
    radius: 3
    clip: true
    color: 'transparent'
    border.color: control.enabled ? '#ccc' : '#d5d5d5'
    TextInput
    {
        id: textInput
        anchors.centerIn: parent
        width: parent.width
        leftPadding: 5
        rightPadding: 5
        selectByMouse: true
        color: control.enabled ? '#333' : '#777'
        selectionColor: '#E91E63'
    }
    MouseArea
    {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        cursorShape: Qt.IBeamCursor
    }
}
