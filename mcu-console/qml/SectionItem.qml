import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle
{
	anchors.fill: parent
	color: '#ccc'
	Text
	{
		anchors.fill: parent
		anchors.leftMargin: 7
		verticalAlignment: Text.AlignVCenter
		text:model.name
		font.capitalization: Font.AllUppercase
		font.bold: true
		font.pixelSize: 12
	}
}
