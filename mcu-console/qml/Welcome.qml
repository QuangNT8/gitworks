import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Item
{
	function openProjectEditor()
	{
		projectEditorLoader.active= false;
		projectEditorLoader.active= true;
	}

	function open(file)
	{
		console.log(file)
		if (Console.open(file))
		{
            //commandModel = Console.commands;
			welcome.visible=false
		}
	}


	MessageDialog
	{
		id: messageDialog
		title: "Error"
		text: "Error in open project"
	}

	Loader
	{
		id: projectEditorLoader
		source: "qrc:/ProjectEditor.qml"
		active: false
	}
	Component
	{
		id: fileComponent
		FileDialog
		{

			visible: true
			id: fileDialog
			title: "Please choose a file"
			folder: shortcuts.home
			onAccepted:
			{
                if (Console.open(Console.convertUrl(fileUrl)))
                {
                    //commandModel = Console.commands;
                    welcome.visible=false
                }
				else messageDialog.open()
			}
		}
	}

	ColumnLayout
	{
		anchors.fill: parent
		anchors.leftMargin: 150
		anchors.rightMargin: 150
		anchors.topMargin: 100
		anchors.bottomMargin: 100
		spacing: 10
		Label
		{
			Layout.fillWidth: true
			text: "Welcome to Firmware Debugger"
			font.pixelSize: 35
		}
		Item
		{
			height: 20
		}

		Label
		{
			Layout.fillWidth: true
			text: "Quick Start"
			font.bold: true
			font.capitalization: Font.AllUppercase
		}

        RowLayout
        {
            Label
            {
                text: 'N'
                font.pixelSize: 30
				font.bold: true
            }
            ToolButton
            {
                text: "New project"
                font.pixelSize: 16
                font.capitalization: Font.MixedCase
				onClicked: openProjectEditor()
            }
        }
		RowLayout
		{
			Loader
			{
				id: openFileLoader
				sourceComponent: fileComponent
				active: false
			}
			Label
			{
				text: 'O'
				font.pixelSize: 30
				font.bold: true
			}
			ToolButton
			{
				text: "Open project"
				font.pixelSize: 16
				font.capitalization: Font.MixedCase
				onClicked:
				{
					openFileLoader.active = false;
					openFileLoader.active = true;
				}
			}
		}

		Label
		{
			Layout.fillWidth: true
			text: "Recent Projects"
			font.bold: true
			font.capitalization: Font.AllUppercase
		}

		Repeater
		{
			model: Console.recentList

			RowLayout
			{

				Label
				{
					text: 'P'
					font.pixelSize: 30
					font.bold: true
				}
				ToolButton
				{
					text: model.name
					font.pixelSize: 16
					font.capitalization: Font.MixedCase
					onClicked: open(model.file)
				}
			}
		}
		Item
		{
			Layout.fillHeight: true
		}
	}
}
