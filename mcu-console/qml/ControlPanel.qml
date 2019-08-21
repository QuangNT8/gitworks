import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.2
import Qt.labs.platform 1.0

ListView
{
	spacing: 0
	focus: true
	id: controlPanel
	keyNavigationEnabled: true
    currentIndex: 0

	Component
	{
		id: folderDialog
		FolderDialog
		{
			visible: true
            folder: Console.commands.folder
            Component.onCompleted: console.log(folder)
			onAccepted:
			{
				Console.generateHeaderFile(currentFolder)
			}
		}
	}
	Loader
	{
		id: folderDialogLoader
		sourceComponent: folderDialog
		active: false
	}

	header: ColumnLayout
	{
		spacing:0
		width: parent.width
        Rectangle
        {
            Layout.fillWidth: true
            height: 1
            color: '#ccc'
        }

		ToolButton
		{
			Layout.fillWidth: true
			text:"Edit Project"
			onClicked: welcome.openProjectEditor()
            Layout.preferredHeight: 30

		}
		Rectangle
		{
			Layout.fillWidth: true
			height: 1
			color: '#ccc'
		}
        ToolButton
        {
            Layout.fillWidth: true
            text:"Close Project"
            Layout.preferredHeight: 30
            onClicked:
            {
                Console.close();
                welcome.visible= true;
            }
        }
        Rectangle
        {
            Layout.fillWidth: true
            height: 1
            color: '#ccc'
        }

        ToolButton
        {
            Layout.fillWidth: true
            text:"Generate Source"
            Layout.preferredHeight: 30
            onClicked:
            {
                folderDialogLoader.active = false
                folderDialogLoader.active = true
            }
        }

        Rectangle
        {
            Layout.fillWidth: true
            height: 1
            color: '#ccc'
        }
	}

	model: commandModel
    delegate:Loader
	{
        width: parent.width
        height: model.type===0?25:40
        source: itemSource(model.type)
		function itemSource(type)
		{
			if (type === 1) return 'qrc:/FieldItem.qml'
			else if (type === 0) return 'qrc:/SectionItem.qml'
			else if (type === 2) return 'qrc:/ButtonItem.qml'
			else if (type === 3) return 'qrc:/PlotItem.qml'
			else if (type === 4) return 'qrc:/PlotHeaderItem.qml'
		}
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                console.log(index)
                controlPanel.currentIndex = index
                controlPanel.forceActiveFocus()
            }
        }

		Rectangle
		{
			anchors.bottom: parent.bottom
			height: 1
			width: parent.width
			color: '#ccc'
		}
	}
}
