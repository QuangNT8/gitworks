import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import '../commons'

BasePage
{
    id: basePage
    property var recipe
    headerTitle: 'Customize'

    footer: Item
    {
        height: 70
        Rectangle
        {
            anchors.fill: parent
            color: '#666'
            opacity: 0.5
        }
        Button
        {
            anchors.centerIn: parent
            Material.accent: Material.Green
            width: parent.width - 10
            height: parent.height - 6
            text: 'Confirm'
            font.pixelSize: 20
            font.bold: true
            highlighted: true
            onClicked: showPage('qrc:/home/SelectMachinePage.qml')
        }
    }
    Flickable
    {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: width
        contentHeight: container.height

        ColumnLayout
        {
            id: container
            spacing: 5
        }
    }
}
