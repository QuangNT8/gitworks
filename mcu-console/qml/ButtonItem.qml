import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item
{
	anchors.fill: parent
	ToolButton
	{
		anchors.fill: parent
		text: model.name
		onClicked: Console.buttonClicked(model.command);
	}
}
