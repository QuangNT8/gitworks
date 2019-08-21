import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Rectangle
{
    width: parent.width
    height: 40
    color: '#ddd'
	function save()
	{
		model.name = txtName.text
		model.color = colorItem.color
		model.highSpeed = txtHighSpeed.checked
	}

    ColorDialog
    {
        visible: false
        id: colorDialog
        title: "Please choose a color"
        onAccepted:
        {
            colorItem.color = color
        }
    }

    RowLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 40
		spacing: 2
        Label
        {
			text: '<b>PLOT</b> { name:"'
            font.family: 'Courier'
        }

		TextItem
		{
			id: txtName
            text: model.name
			KeyNavigation.tab: colorItem
			KeyNavigation.right: colorItem
        }

        Label
        {
			text: '"; color :'
            font.family: 'Courier'
        }

        ToolButton
        {
            id: colorItem
            property alias color: recColor.color
            Layout.preferredWidth: 50
            Rectangle
            {
                id: recColor
                width: 20
                height: 20
				color: model.color
                anchors.centerIn: parent
            }
            onClicked:
            {
                colorDialog.open()
            }
			KeyNavigation.tab: txtHighSpeed
			KeyNavigation.right: txtHighSpeed
        }

		Label
		{
			text: '; highSpeed :'
			font.family: 'Courier'
		}

		CheckBox
		{
			id: txtHighSpeed
			checked: model.highSpeed
			KeyNavigation.tab: txtName
			Layout.preferredHeight: 40
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
            focus: false
            text: '↑'
            onClicked:
            {
                save()
                editModel.movePlotUp(model)
            }
        }

        ToolButton
        {
            focus: false
            text: '↓'
            onClicked:
            {
                save()
                editModel.movePlotDown(model)
            }
        }

		ToolButton
		{
			text: '-'
			onClicked:
			{
				editModel.remove(model)
			}
		}
    }
}
