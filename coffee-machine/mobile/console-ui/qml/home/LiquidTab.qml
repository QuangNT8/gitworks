import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.3
import'../commons'

BasePage
{
    id: liquidTab
    isHomePage: true
    property var fileTraining: ""

    Flickable
    {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: width
        contentHeight: liquidGrid.height
        GridLayout
        {
            id: liquidGrid
            width: parent.width
            columns: 5
            columnSpacing: 5
            rowSpacing: 5
            Rectangle
            {
                Layout.preferredWidth: (parent.width-(liquidGrid.columns-1)*liquidGrid.columnSpacing)/liquidGrid.columns
                Layout.preferredHeight: width
                color: '#ccc'
                border.color: '#ccc'
                radius: 3
                Label
                {
                    anchors.centerIn: parent
                    text: Flaticon.get('add')
                    font.family: 'Flaticon'
                    font.pixelSize: 60
                    color: '#666'
                }
                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onPressed: parent.color = '#bbb'
                    onReleased: parent.color = '#ccc'
                    onClicked:
                    {
                        liquidDetailDialog.currentLiquid = null
                        liquidDetailDialog.open()
                    }
                }
            }
            Repeater
            {
                id: repeater
                model: Console.liquids
                delegate: Rectangle
                {
                    Layout.preferredWidth: (parent.width-(liquidGrid.columns-1)*liquidGrid.columnSpacing)/liquidGrid.columns
                    Layout.preferredHeight: width
                    color: '#ccc'
                    radius: 3
                    Image
                    {
                        anchors.fill: parent
                        anchors.margins: 10
                        asynchronous: true
                        fillMode: Image.PreserveAspectFit
                        source: 'qrc:/images/liquid-default-background.png'
                        opacity: 0.25
                    }

                    Label
                    {
                        anchors.centerIn: parent
                        width: parent.width - 20
                        text: model.name
                        color: '#444'
                        font.pixelSize: 16
                        font.bold: true
                        maximumLineCount: 2
                        wrapMode: Label.Wrap
                        elide: "ElideRight"
                        horizontalAlignment: Text.AlignHCenter
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            liquidDetailDialog.currentLiquid = model
                            liquidDetailDialog.open()
                        }
                    }
                }
            }
            Item
            {
                Layout.fillWidth: true
                visible: repeater.count < liquidGrid.columns
            }
        }
    }

    LiquidDetailDialog
    {
        id: liquidDetailDialog
    }

    BaseDialog
    {
        id: addLiquidDialog
        parent: home
        title: 'Add new liquid'
        height: contentHeight + 150
        contentItem: ColumnLayout
        {
            width: parent.width
            spacing: 10
            RowLayout
            {
                Layout.fillWidth: true
                spacing: 15
                Label
                {
                    text: 'Name'
                }
                TextField
                {
                    id: liquidNameInput
                    Layout.fillWidth: true
                    text: "New liquid"
                    placeholderText: 'Enter liquid name'
                }
            }
            RowLayout
            {
                spacing: 15
                Label
                {
                    text: 'Data:'
                    font.pixelSize: 15
                    color: '#333'
                }
                Label
                {
                    id: fileName
                    Layout.fillWidth: true
                    text: fileTraining === "" ? '[No file selected]' : fileTraining
                    font.pixelSize: 15
                    font.italic: true
                    color: '#666'
                    elide: "ElideLeft"
                }
                Button
                {
                    Layout.preferredHeight: 45
                    text: 'Select'
                    onClicked: fileDialog.open()
                }
            }
        }
        onAccepted:
        {
            addLiquid()
            liquidNameInput.clear()
            fileTraining = ""
        }
        onRejected:
        {
            liquidNameInput.clear()
            fileTraining = ""
        }
    }

    FileDialog
    {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: fileTraining = fileDialog.fileUrl
    }
}
