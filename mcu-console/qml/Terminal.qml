import QtQuick 2.7
import QtQuick.Controls 2.0

Flickable
{
    contentHeight: message.height
    contentWidth: width
    clip: true
    ScrollBar.vertical: ScrollBar {}
    function clear()
    {
        message.clear()
    }

    function append(text)
    {
        if (message.length > 5000) message.remove(0, 2500)
        message.append(text);
        if (contentHeight > height) contentY = contentHeight - height
    }

    TextEdit
    {
        id: message
        width: parent.width
        color: '#fff'
        readOnly: true;
        selectionColor: "#2196F3"
        selectByKeyboard: true
        selectByMouse: true
        textFormat: TextEdit.PlainText
        wrapMode: TextEdit.WordWrap
    }
}

