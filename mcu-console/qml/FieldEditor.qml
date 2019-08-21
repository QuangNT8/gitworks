import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle
{
    width: parent.width
	height: 40
    color: '#ddd'
	function save()
	{
		model.name = txtName.text
		model.min = parseInt(txtMin.text,10)
		model.max = parseInt(txtMax.text,10)
		model.value = parseInt(txtValue.text,10)
		model.readonly = txtReadonly.checked
	}

    RowLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 40
        spacing: 2
        Label
        {
			text: '<b>FIELD</b> { name: "'
            font.family: 'Courier'
        }

		TextItem
        {
			id: txtName
            text: model.name
			KeyNavigation.tab: txtMin
			KeyNavigation.right: txtMin
        }
        Label
        {
			text: '"; min: '
            font.family: 'Courier'
        }
		NumberItem
        {
			id: txtMin
			text: model.min
			KeyNavigation.tab: txtMax
			KeyNavigation.right: txtMax
        }
        Label
        {
			text: '; max: '
            font.family: 'Courier'
        }
		NumberItem
        {
			id: txtMax
			text: model.max
			KeyNavigation.tab:  txtValue
			KeyNavigation.right:  txtValue
        }

        Label
        {
            text: '; value: '
            font.family: 'Courier'
        }
		NumberItem
        {
			id: txtValue
			text: model.value
			KeyNavigation.tab: txtReadonly
			KeyNavigation.right: txtReadonly
        }

        Label
        {
            text: '; readonly: '
            font.family: 'Courier'
        }
        CheckBox
        {
			id:txtReadonly
			checked: model.readonly
			Layout.maximumWidth: 40
			Layout.maximumHeight: 40
			KeyNavigation.tab: txtName
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
			focus: false
			text: '-'
			onClicked:
			{
				editModel.remove(model)
			}
		}
    }
}
