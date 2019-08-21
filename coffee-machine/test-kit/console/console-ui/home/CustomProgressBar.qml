import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12

ProgressBar
{
    id: control
    width: 350
    background: Rectangle
    {
        implicitWidth: 200
        implicitHeight: 6
        color: "#e6e6e6"
    }

    contentItem: Item
    {
        implicitWidth: 200
        implicitHeight: 6
        Rectangle
        {
            width: control.visualPosition * parent.width
            height: parent.height
            color: Material.color(Material.Green)
            opacity: 0.8
        }
    }
}
