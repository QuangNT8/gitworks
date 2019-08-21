import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ColumnLayout
{
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    Item {
        height: 1
    }

    function setRange(min, max)
    {
        spinZero.from = min;
        spinZero.to = max;

    }

    RowLayout
    {
        Layout.fillWidth: true
        ComboBox
        {
            Layout.fillWidth: true
            model: Console.commands
            textRole: "name"
        }
        Rectangle
        {
            width: 60
            height: 35
            radius: 3
            TextInput
            {
                id: txtZero
                anchors.fill: parent
                verticalAlignment: TextInput.AlignVCenter
                leftPadding: 10
                rightPadding: 10
                validator: IntValidator{top: slideZero.to; bottom: slideZero.from}
                text: slideZero.value
                onTextChanged:
                {
                    slideZero.value = parseInt(text)
                }
            }
        }

        Button
        {
            text: "Analyse"
        }
    }

    RowLayout
    {
        Layout.fillWidth: true
        Slider
        {
            id:slideZero
            Layout.fillWidth: true
            from: 0
            to: 4096
            onValueChanged: txtZero.text = value
        }

    }

    GridLayout
    {
        columns: 4
        Label
        {
            Layout.fillWidth: true
            text: 'min'
            horizontalAlignment: "AlignHCenter"
        }
        Label
        {
            Layout.fillWidth: true
            text: 'max'
            horizontalAlignment: "AlignHCenter"
        }

        Label
        {
            Layout.fillWidth: true
            text: 'width'
            horizontalAlignment: "AlignHCenter"
        }

        Label
        {
            Layout.fillWidth: true
            text: 'rms'
            horizontalAlignment: "AlignHCenter"
        }
        Label
        {
            Layout.fillWidth: true
            text: '0'
            horizontalAlignment: "AlignHCenter"
        }
        Label
        {
            Layout.fillWidth: true
            text: '0'
            horizontalAlignment: "AlignHCenter"
        }
        Label
        {
            Layout.fillWidth: true
            text: '0'
            horizontalAlignment: "AlignHCenter"
        }
        Label
        {
            Layout.fillWidth: true
            text: '0'
            horizontalAlignment: "AlignHCenter"
        }
    }
    Item {
        height: 8
    }
}
