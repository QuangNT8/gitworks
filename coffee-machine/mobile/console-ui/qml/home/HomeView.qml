import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import '../commons'

Page
{
    id: home

    function sendLogDebug(log) {logView.printMessage(Console.getTime(), "D", log)}
    function sendLogError(log) {logView.printMessage(Console.getTime(), "E", log)}
    function sendLogInfo(log) {logView.printMessage(Console.getTime(), "I", log)}
    function sendLogWarning(log) {logView.printMessage(Console.getTime(), "W", log)}

    Material.primary: Material.Brown

    background: Rectangle
    {
        color: 'white'
    }
    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        ColumnLayout
        {
            id: functionSection
            Layout.fillWidth: true
            spacing: 0
            TabBar
            {
                id: bar
                z:100
                position: TabBar.Header
                Layout.preferredWidth: 460
                Layout.preferredHeight: 45
                Material.accent: Material.Brown
                TabButton
                {
                    Label
                    {
                        anchors.centerIn: parent
                        text: 'Machine'
                        font.capitalization: Font.AllUppercase
                        color: parent.checked ? Material.color(Material.Brown): '#aaa'
                        font.bold: parent.checked ? true: false
                    }
                }
                TabButton
                {
                    Label
                    {
                        anchors.centerIn: parent
                        text: 'Liquid'
                        font.capitalization: Font.AllUppercase
                        color: parent.checked ? Material.color(Material.Brown): '#aaa'
                        font.bold: parent.checked ? true: false
                    }
                }
                TabButton
                {
                    Label
                    {
                        anchors.centerIn: parent
                        text: 'Recipe'
                        font.capitalization: Font.AllUppercase
                        color: parent.checked ? Material.color(Material.Brown): '#aaa'
                        font.bold: parent.checked ? true: false
                    }
                }
            }

            StackLayout
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 5
                currentIndex: bar.currentIndex
                clip: true
                MachineTab
                {
                    id: configTab
                }
                LiquidTab
                {
                    id: liquidTab
                }
                RecipeTab
                {
                    id: recipeTab
                }
            }
        }
        LogView
        {
            id: logView
        }
    }
}
