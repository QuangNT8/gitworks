import QtQuick 2.7

TextInput
{
	focus: true
	selectByMouse: true
	height: 40
	verticalAlignment: TextInput.AlignVCenter
	validator: IntValidator
	{
		bottom: -32000
		top: 32000
	}
	onActiveFocusChanged:
	{
		if (activeFocus) selectAll()
	}
}
