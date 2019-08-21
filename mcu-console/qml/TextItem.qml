import QtQuick 2.7

TextInput
{
	focus: true
	selectByMouse: true
	height: 40
	verticalAlignment: TextInput.AlignVCenter
	onActiveFocusChanged:
	{
		if (activeFocus) selectAll()
	}
}
