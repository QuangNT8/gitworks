import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import '../commons'

Rectangle
{
    id: consoleLogSection
    Layout.preferredWidth: 350
    Layout.fillHeight: true
    color: '#2B2B2B'
    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        anchors.topMargin: 5
        spacing: 5
        RowLayout
        {
            Layout.fillWidth: true
            spacing: 5
            Label
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                text: 'LOGS'
                color: 'white'
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Button
            {
                text: 'ResetMachine'
                onClicked: resetMachine()
            }
            Button
            {
                text: 'Clear'
                onClicked: listModel.clear()
            }
        }
        Rectangle
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: 'white'
        }
        //Log Tag: D: Debug, I: Info, E: Error

        ListView
        {
            id: listLog
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: listModel
            spacing: 5
            clip: true
            delegate: Label
            {
                id: lblLog
                width: parent.width
                text:
                {
                    if (model.tag === "D") return model.time + " [Debug] " + model.message;
                    if (model.tag === "I") return model.time + " [Info] " + model.message;
                    if (model.tag === "E") return model.time + " [Error] " + model.message;
                    if (model.tag === "W") return model.time + " [Warning] " + model.message;
                    return model.message;
                }

                wrapMode: Text.WordWrap
                maximumLineCount: 5
                color:
                {
                    if (model.tag === "D") return "#EEEEEE"; //tag: D-Debug
                    if (model.tag === "I") return "#4CAF50"; //tag: I-Info
                    if (model.tag === "E") return "#F44336"; //tag: E-Error
                    if (model.tag === "W") return "#FFC107"; //tag: W-Warning
                    return "#EEE";
                }
            }
            ScrollBar.vertical: ScrollBar { width: 5; policy: ScrollBar.AlwaysOn }

            ListModel
            {
                id: listModel
            }
        }
    }

    Connections
    {
        target: Console
        onMessageReceived:
        {
            listModel.append({"message": log, "tag": tag, "time": time})
            listLog.positionViewAtEnd()
        }
    }

    function printMessage(time, tag, log)
    {
        listModel.append({"message": log, "tag": tag, "time": time})
        listLog.positionViewAtEnd()
    }
    function resetMachine()
    {
        var op = Engine.createOperation("machine.ResetMachine")
        op.fire();
    }
}
