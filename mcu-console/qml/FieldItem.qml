import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

RowLayout
{
	anchors.fill: parent
	Label
	{
		Layout.preferredWidth: 80
		text: model.name
		horizontalAlignment: "AlignRight"
	}
	Rectangle
	{
		Layout.fillHeight: true
		width: 1
		color: '#ccc'
	}
	TextInput
	{
        focus: true
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.leftMargin: 3
		Layout.rightMargin: 3
		text: model.value
        font.bold: model.readonly
        color: model.readonly?"#666":"#222"

        selectByMouse: true
		verticalAlignment: TextInput.AlignVCenter
		validator: IntValidator
		{
			bottom: model.min
			top: model.max
		}
		onActiveFocusChanged:
		{
			if (activeFocus & !readOnly) selectAll()
		}
		onAccepted:
		{
			Console.setVariable(model.command, text)
		}
		readOnly: model.readonly
	}
}
