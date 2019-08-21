import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

BusyIndicator
{
    property int ringSize: 32
    property int nutSize: 6
    id: control
    contentItem: Item
    {
        implicitWidth: ringSize
        implicitHeight: ringSize

        Item
        {
            id: item
            x: parent.width / 2 - width/2
            y: parent.height / 2 - height/2
            width: ringSize
            height: ringSize
            opacity: control.running ? 1 : 0

            Behavior on opacity {
                OpacityAnimator
                {
                    duration: 250
                }
            }

            RotationAnimator
            {
                target: item
                running: control.visible && control.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 3000
            }

            Repeater
            {
                id: repeater
                model: 6

                Rectangle {
                    x: item.width / 2 - width / 2
                    y: item.height / 2 - height / 2
                    implicitWidth: nutSize
                    implicitHeight: nutSize
                    radius: nutSize/2
                    color: Material.color(Material.Brown)
                    transform: [
                        Translate
                        {
                            y: -Math.min(item.width, item.height) * 0.5 + 5
                        },
                        Rotation
                        {
                            angle: index / repeater.count * 360
                            origin.x: nutSize/2
                            origin.y: nutSize/2
                        }
                    ]
                }
            }
        }
    }
}
