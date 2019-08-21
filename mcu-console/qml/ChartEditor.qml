import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle
{
    width: parent.width
    height: 40
    color: '#bbb'
	function save()
	{
		model.min = parseInt(txtMin.text,10);
		model.max = parseInt(txtMax.text,10);
		model.visible = txtVisible.checked
	}
	RowLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 10
        Label
        {
            text: model.name
            font.bold: true
            font.capitalization: Font.AllUppercase
            font.family: 'Courier'
        }

        Label
        {
            text: ' { yMin:'
            font.family: 'Courier'
        }

		TextItem
        {
			id: txtMin
			text: model.min
			KeyNavigation.tab: txtMax
			KeyNavigation.right: txtMax
        }

        Label
        {
            text: '; yMax:'
            font.family: 'Courier'
        }

		TextItem
        {
			id: txtMax
			text: model.max
			KeyNavigation.tab: txtVisible
			KeyNavigation.right: txtVisible
        }

		Label
		{
			text: '; visible:'
			font.family: 'Courier'
		}

		CheckBox
		{
			id: txtVisible
			checked: model.visible
			KeyNavigation.tab: txtMin
		}

        Label
        {
            text: '}'
            font.family: 'Courier'
        }
		Item
		{
            Layout.fillWidth: true
        }

        ToolButton
        {
            text: '+P'
            Layout.preferredHeight: 40
			onClicked:
			{
				editModel.newPlot(model,"New Plot")
			}

        }

    }
}
