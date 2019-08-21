import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog
{
    id: projectEditor
    visible: true
    title: "Edit Project"
    modality: Qt.ApplicationModal
    property var editModel: Console.editModel()

    function save()
    {
        editList.save()
        editModel.save()
        Console.swapModel(editModel)
        //commandModel = editModel
        welcome.visible=false
        projectEditor.close()
    }

    contentItem: Rectangle
    {
        color: "#ddd"
        implicitWidth: 1000
        implicitHeight: 800

        ColumnLayout
        {
            spacing: 0
            anchors.fill: parent

            ListView
            {
                id: editList
                clip: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: editModel
                keyNavigationEnabled: true
                focus: true
                currentIndex: 0

                function save()
                {
                    for(var child in contentItem.children)
                    {
                        if (contentItem.children[child].item && contentItem.children[child].item.save)
                        {
                            contentItem.children[child].item.save()
                        }
                    }
                }

                header: Rectangle
                {
                    function save()
                    {
                        if (txtLocation.text.length>0) editModel.filePath = txtLocation.text
                        else editModel.filePath='';
                        editModel.projectName = txtProjectName.text
                    }

                    width: parent.width
                    height: 40
                    color:'#aaa'
                    focus: true
                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.leftMargin: 10;
                        spacing: 2
                        Label
                        {
                            text: '<b>PROJECT</b> { name: "'
                            font.family: 'Courier'
                        }
                        TextItem
                        {
                            id: txtProjectName
                            text: editModel.projectName
                            KeyNavigation.tab: txtProjectName
                        }

                        Label
                        {
                            text: '", location: "'
                            font.family: 'Courier'
                        }
                        Label
                        {
                            id:txtLocation
                            text: editModel.filePath
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
                            Layout.preferredHeight: 40
                            text: '+S'
                            onClicked:
                            {
                                editModel.newSection("New Section")
                            }
                        }
                    }
                }

                delegate: Loader
                {
                    width: parent.width
                    source: itemComponent(model.type)
                    function itemComponent(type)
                    {
                        if (type ===1 ) return 'qrc:/FieldEditor.qml';
                        else if (type === 2 ) return 'qrc:/ButtonEditor.qml';
                        else if (type === 4 ) return 'qrc:/ChartEditor.qml';
                        else if (type === 3 ) return 'qrc:/PlotEditor.qml';
                        return 'qrc:/SectionEditor.qml';
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            editList.currentIndex = index
                            editList.forceActiveFocus()
                        }
                    }
                }
            }

            Rectangle
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 65
                color: '#aaa'

                RowLayout
                {
                    anchors.fill: parent
                    anchors.margins: 10

                    Item
                    {
                        Layout.fillWidth: true
                    }

                    Button
                    {
                        text: 'Cancel'
                        onClicked: projectEditor.close()
                    }

                    Button
                    {
                        FileDialog
                        {
                            id: fileDialog
                            selectExisting: false
                            title: "Please choose a file"
                            folder: shortcuts.home
                            visible: false
                            onAccepted:
                            {
                                editModel.filePath = Console.convertUrl(fileDialog.fileUrl)
                                projectEditor.save();
                            }
                        }
                        MessageDialog
                        {
                            id: messageDialog
                            title: "Error"
                            text: "Invalid project location, please choose a correct one"
                        }
                        text: 'Save'
                        onClicked:
                        {
                            editList.headerItem.save();
                            if (editModel.filePath.length>0) projectEditor.save();
                            else
                            {
                                fileDialog.open();
                            }
                        }
                    }
                }
            }
        }
    }
}
