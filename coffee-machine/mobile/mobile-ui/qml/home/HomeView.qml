import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

Page
{
    Material.primary: Material.Brown
    background: Rectangle
    {
        color: 'white'
    }
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0
        Rectangle
        {
            z:100
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            color: '#f8f8f8'
            RowLayout
            {
                anchors.fill: parent
                anchors.leftMargin: 15
                Label
                {
                    id: title
                    Layout.fillWidth: true
                    text: 'Brain Juice Mobile'
                    font.bold: true
                    color: '#333'
                    font.pixelSize: 20
                }
            }
            Rectangle
            {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: '#eee'
            }
        }

        StackLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex
            clip: true
            MenuTab
            {
                id: menuTab
            }
        }
        TabBar
        {
            id: bar
            z:100
            height: 60
            position: TabBar.Footer
            Layout.fillWidth: true
            Layout.preferredHeight: 65
            Material.accent: Material.DeepPurple
            visible: false //FIXME: hide on prototype
            HomeTabItem
            {
                symbol: 'H'
                title: 'Home'
            }
            HomeTabItem
            {
                symbol: 'A'
                title: 'Account'
            }
            HomeTabItem
            {
                symbol: 'N'
                title: 'News'
            }
            HomeTabItem
            {
                symbol: 'S'
                title: 'Search'
            }
        }
    }
}
