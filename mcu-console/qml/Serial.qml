import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.2

Rectangle
{
    color: '#444'
    ColumnLayout
    {
        anchors.margins: 10
        anchors.fill: parent

        RowLayout
        {
            Layout.fillWidth: true
            ComboBox
            {
                id: port
                Layout.fillWidth: true
                width: parent.width
                model: SerialPort.ports()
                enabled: btConnect.text == 'Connect'
                visible: btConnect.visible
            }

            Button
            {
                text: 'Pause';
                onClicked:
                {
                    if (text === 'Pause')
                    {
                        Console.pause(true);
                        text ='Resume'
                    }
                    else
                    {
                        Console.pause(false);
                        text ='Pause'
                    }
                }
                visible: btDisconnect.visible
                Layout.fillWidth: true
            }

            Button
            {
                text: 'Clear';
                onClicked: txtConsole.clear()
                visible: btDisconnect.visible
            }

            Button
            {
                text: 'Refresh';
                onClicked:port.model = SerialPort.ports()
                visible: btConnect.visible
            }

            Button
            {
                id: btConnect
                text: 'Connect';
                onClicked:
                {
                    if (SerialPort.open(port.currentText))
                    {
                        btConnect.visible = false
                        Views.showMessage("SerialPort Connected")
                    }
                    else
                    {
                        Views.showMessage("Can not connect to SerialPort")
                    }
                }
            }

            Button
            {
                id: btDisconnect
                text: 'Disconnect';
                visible: !btConnect.visible
                onClicked:
                {
                    SerialPort.close();
                    btConnect.visible = true
                    Views.showMessage("SerialPort Disconnected")
                }
            }
        }

        RowLayout
        {
            Layout.fillWidth: true
            Rectangle
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                border.width: 1
                border.color: '#666'
                color: "#444"
                Label
                {
                    anchors.centerIn: parent
                    text:SerialPort.freq + ' Hz'
                    color: '#ddd'
                    font.pixelSize: 18

                }
            }
            Rectangle
            {
                border.width: 1
                border.color: '#666'
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                color: "#444"
                Label
                {
                    anchors.centerIn: parent
                    text:SerialPort.bandwidth + ' bytes/s'
                    color: '#ddd'
                    font.pixelSize: 18
                }
            }
        }
    }
}
