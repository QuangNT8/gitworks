import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle
{
	anchors.fill: parent
	color: '#ccc'
	RowLayout
	{
		anchors.fill: parent
		Label
		{
			text: model.name
			//anchors.fill: parent
			Layout.fillWidth: true
			Layout.leftMargin: 7
			Layout.fillHeight: true
			//anchors.leftMargin: 5
			verticalAlignment: Text.AlignVCenter
			font.capitalization: Font.AllUppercase
			font.bold: true
			font.pixelSize: 12
		}
		ToolButton
		{
            Component.onCompleted:
            {
                displayPlot(model.index, model.visible)
            }
            text: model.visible?'hide':'show'
			Layout.fillHeight: true
			font.capitalization: Font.AllUppercase
			font.pixelSize: 11
			onClicked:
			{
				if (text =='hide')
				{
					displayPlot(model.index, false)
					text = 'show'
				}
				else
				{
					displayPlot(model.index, true)
					text = 'hide'
				}
			}
		}
	}
}
