import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

RowLayout
{
	anchors.fill: parent
	spacing: 0
	CheckBox
	{
		Layout.fillWidth: true
		Layout.fillHeight: true
		checked: true
		text: model.name
        onCheckedChanged: model.plot.visible = checked
	}
	Rectangle
	{

        height: 20
        width: 20
        Layout.rightMargin: 8
		color: model.color
	}
}
