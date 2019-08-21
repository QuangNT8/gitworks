import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import '../commons'

BaseItem
{
    Layout.fillWidth: true
    Layout.preferredHeight: container.height + bottomPadding + 10
    ColumnLayout
    {
        id: container
        width: parent.width
        spacing: 5
        Item
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 45

            RowLayout
            {
                anchors.fill: parent
                RoundButton
                {
                    text: Flaticon.get('back')
                    font.family: 'Flaticon'
                    flat: true
                    onClicked: back()
                }
                Label
                {
                    Layout.fillWidth: true
                    text: recipe.name
                    color: Material.color(Material.Brown)
                    font.pixelSize: 18
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    elide: "ElideRight"
                }
                RoundButton
                {
                    Label
                    {
                        anchors.centerIn: parent
                        text: isFavorite ? Flaticon.get('heart-bold') : Flaticon.get('heart')
                        font.family: 'Flaticon'
                        color: isFavorite ? 'red': '#333'
                    }
                    flat: true
                    onClicked: isFavorite = !isFavorite
                }
            }
        }
        Item
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 100
            Layout.preferredHeight: 100
            Image
            {
                id: img
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                source: imgSource
                sourceSize: Qt.size(parent.width, parent.height)
                layer.enabled: true
                layer.effect: OpacityMask
                {
                    maskSource: Item
                    {
                        width: img.width
                        height: img.height
                        Rectangle
                        {
                            anchors.centerIn: parent
                            width: parent.width
                            height: parent.height
                            radius: width/2
                        }
                    }
                }
            }
        }

        Label
        {
            Layout.fillWidth: true
            text: "by <i>" + "Han's Coffee" + "</i>"
            font.pixelSize: 15
            color: '#333'
            horizontalAlignment: Text.AlignHCenter
        }
        Label
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: recipe.description==="" ? 'No description' : recipe.description
            font.pixelSize: 13
            maximumLineCount: 5
            color: '#666'
            wrapMode: "WordWrap"
            elide: "ElideRight"
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
