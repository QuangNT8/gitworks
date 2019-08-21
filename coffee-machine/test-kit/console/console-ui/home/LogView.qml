import QtQuick 2.12
import QtQuick.Controls 2.5

ListView
{
    id: listLog
    anchors.fill: parent
    spacing: 5
    model: listModel
    delegate: Label
    {
        id: lblLog
        width: parent.width
        text:model.time+ " "+ model.message
        wrapMode: Text.WordWrap
        maximumLineCount: 5
        color:"white"
    }

    ListModel
    {
        id: listModel
    }

    Connections
    {
        target: Kit
        onMessageReceived:
        {
            listModel.append({"message": log, "time": time})
            listLog.positionViewAtEnd()
        }
    }
}
