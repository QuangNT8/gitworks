import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

RowLayout
{
	id: sectionTitle
	signal clicked()
    property alias text: txtTitle.text
    property alias icon: btRight.text
    Layout.fillWidth: true
    Layout.preferredHeight: 50
    Label
    {
        id: txtTitle
        Layout.topMargin: 4
        Layout.fillWidth: true
        Layout.fillHeight: true
        font.capitalization: Font.AllUppercase
        verticalAlignment: "AlignVCenter"
        font.bold: true
        font.pixelSize: 13
        color: Material.color(Material.Pink)
    }

//    RoundButton
//    {
//        id: btRight
//        Layout.topMargin: 4
//        Layout.fillHeight: true
//        Layout.rightMargin: -10
//        text: Flaticon.get('hmore')
//        font.family: 'Flaticon'
//        font.pixelSize: 18
//        flat: true
//		onClicked: sectionTitle.clicked()
//        visible: text.length > 0
//        Material.foreground: Material.color(Material.Pink)
//    }
    ToolButton
    {
        id: btRight
        Layout.topMargin: 4
        Layout.fillHeight: true
        Layout.rightMargin: -9
        Layout.preferredWidth: height
        text: Flaticon.get('hmore')
        font.family: 'Flaticon'
        font.pixelSize: 18
        flat: true
        onClicked: sectionTitle.clicked()
        visible: text.length > 0
        Material.foreground: Material.color(Material.Pink)
    }
}
