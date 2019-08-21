import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

BaseItem
{
    Layout.fillWidth: true
    Layout.preferredHeight: timerArea.height + 24

    RowLayout
    {
        id: timerArea
        width: parent.width
        spacing: 11
        GridLayout
        {
            columns: 2
            columnSpacing: 11
            Label
            {
                Layout.alignment: Qt.AlignRight
                text: 'will'
                color: '#666'
            }

            Rectangle
            {
                height: turnoff.height
                width: turnoff.width
                Layout.fillWidth: true
                //border.color: '#bbb'
                color: '#eee'
                radius: 4

                ToolButton
                {
                    id: turnoff
                    anchors.centerIn: parent
                    anchors.fill: parent
                    text: 'TURN OFF'
                    font.bold: true
                    font.pixelSize: 13
                    Material.foreground: '#666'

                    MouseArea
                    {
                        anchors.fill: parent
                        //onClicked: turnoffDialog.open()
                    }
                }
            }

            Label
            {
                Layout.alignment: Qt.AlignRight
                text: 'in'
                color: '#666'
            }

            Rectangle
            {
                height: time.height
                width: time.width
                Layout.fillWidth: true
                //border.color: '#bbb'
                color: '#eee'
                radius: 4

                ToolButton
                {
                    id: time
                    anchors.centerIn: parent
                    anchors.fill: parent
                    text: '15 minutes'
                    font.bold: true
                    font.pixelSize: 13
                    Material.foreground: '#666'
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {/*TODO:*/}
                    }
                }
            }
        }
        Rectangle
        {
            Layout.fillHeight:  true
            Layout.preferredWidth: 1
            color: '#ddd'
        }
        Button
        {
            id: button
            Layout.preferredWidth: 100
            Layout.fillHeight: true
            text: 'Start'
            highlighted: (button.text ==='Start')?false:true
            onClicked:
            {
                if (button.text === 'Start') button.text ='Stop'
                else button.text ='Start'
                /*TODO: */
            }
        }
    }
}
