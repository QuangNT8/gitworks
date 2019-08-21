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
		model.name = txtName.text
	}
    RowLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 10
        spacing: 2
        Label
        {
			text: '<b>SECTION</b> { name: "'
            font.family: 'Courier'
        }

		TextItem
        {
			id: txtName
            text: model.name
			KeyNavigation.tab: txtName
        }
        Label
        {
			text: '"}'
            font.family: 'Courier'
        }
        Item
        {
            Layout.fillWidth: true
        }

        ToolButton
        {
            text: '+F'
            Layout.maximumHeight: 40
			onClicked:
			{
				editModel.newField(model,"New Field")
			}
        }

        ToolButton
        {
            text: '+B'
            Layout.maximumHeight: 40
			onClicked:
			{
				editModel.newButton(model,"New Button")
			}
        }
        ToolButton
        {
            focus: false
            text: '↑'
            onClicked:
            {
                save()
                editModel.moveUp(model)
            }
        }

        ToolButton
        {
            focus: false
            text: '↓'
            onClicked:
            {
                save()
                editModel.moveDown(model)
            }
        }

		ToolButton
		{
			text: '-'
			onClicked:
			{
				editModel.remove(model)
			}
			Layout.maximumHeight: 40
		}
    }
}
