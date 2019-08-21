import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

Slider
{
    property string handlerIcon : ''
    property var baseValue
    id: control
    implicitHeight: 50
    stepSize: 1
    snapMode: "SnapOnRelease"
    background: Rectangle
    {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: control.enabled ? 4 :2
        width: control.availableWidth
        height: implicitHeight
        radius: 2
        color: control.enabled ? "#bdbebf" : '#ccc'
        Rectangle
        {
            width: control.visualPosition * parent.width
            height: parent.height
            color: control.enabled ? Material.color(Material.Pink, Material.Shade600) : '#aaa'
            radius: 2
        }
    }

    handle: Rectangle
    {
        id: handler
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: control.pressed ? 14 : 20
        implicitHeight: control.pressed ? 14 : 20
        radius: implicitWidth/2
        color: control.pressed ? '#eee' : Material.color(Material.Pink)
        border.color: control.pressed ? '#ccc' : 'transparent'
        Text
        {
            anchors.centerIn: parent
            text: control.pressed ? handlerIcon : control.value
            font.family: 'Flaticon'
            font.pixelSize: parent.width*0.45
            color: '#fff'
            font.bold: true
            visible: true
            RotationAnimation on rotation
            {
                id: increaseAnimate
                running: false
                from: 0
                to: 360
                duration: 400
            }
            RotationAnimation on rotation
            {
                id: decreaseAnimate
                running: false
                from: 0
                to: -360
                duration: 400
            }
        }
    }
    Rectangle
    {
        id: monitor
        x: handler.x + (handler.width-width)/2
        y: handler.y - handle.height
        width: 36
        height: width
        radius: width/2
        border.color: /*'#aaa'*/Material.color(Material.Pink, Material.Shade600)
        color: '#eee'
        visible: control.pressed
        Text
        {
            anchors.centerIn: parent
            text: control.value
            font.pixelSize: parent.width/2 - 1
            font.bold: control.pressed
            color: Material.color(Material.Pink, Material.Shade600)
        }
        states: State {
            name: "moved"; when: control.pressed
            PropertyChanges { target: monitor; x: handler.x + (handler.width-width)/2; y: handler.y - 10 - monitor.height}
        }
        transitions: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuart}
        }
    }
    onPressedChanged:
    {
        baseValue= value
    }
    onValueChanged:
    {
        if (value > baseValue) increaseAnimate.start()
        else if (value < baseValue) decreaseAnimate.start()
    }
}

