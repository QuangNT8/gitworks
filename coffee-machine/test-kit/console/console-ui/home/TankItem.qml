import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import "../commons"

Rectangle
{
    property int tankState: 0
    Layout.preferredWidth: 250
    Layout.preferredHeight: 350
    color:  '#ccc'
    radius: 3

    function editTank()
    {
        var liquid = Console.getLiquidByName(liquidType.currentText);
        if (liquid !== 0 && (tankVolumeInput.length===0 || tankVolumeInput.text < 100 || tankVolumeInput.text > 2000))
        {
            sendLogError("Value invalid for volume tank " + model.index)
            showPageError('Please check input value', 3000)
        }
        else
        {
            var op = Engine.createOperation("machine.ChangeTank")
            op.index = model.index;
            op.volume = liquid !== 0 ? tankVolumeInput.text : 0;
            op.liquid = liquid
            op.fire();
        }
    }

    Rectangle
    {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 45
        color: '#555'
        radius: 3
        Rectangle
        {
            width: parent.width
            height: parent.radius
            color: parent.color
        }
        RowLayout
        {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            Label
            {
                Layout.alignment: Qt.AlignHCenter
                text: '1850' + 'ml'
                font.bold: true
                color: '#ccc'
                font.pixelSize: 14
                visible: tankState === 0
            }
            Label
            {
                Layout.alignment: Qt.AlignHCenter
                text: 'Pouring...'
                font.bold: true
                color: 'red'
                font.pixelSize: 14
                visible: tankState === 1
            }
            Label
            {
                Layout.alignment: Qt.AlignHCenter
                text: 'Measuring...'
                font.bold: true
                color: 'blue'
                font.pixelSize: 14
                visible: tankState === 2
            }
            Label
            {
                Layout.alignment: Qt.AlignHCenter
                text: 'Filling...'
                font.bold: true
                color: 'green'
                font.pixelSize: 14
                visible: tankState === 3
            }
        }
    }

    Rectangle
    {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        width: parent.width
        color: Material.color(Material.LightGreen)
        height: /*Math.round((model.volume/2000 * (parent.height-45)))*/Math.round((750/2000 * (parent.height-45)))
        Behavior on height
        {
            NumberAnimation { duration: 750 }
        }
    }

    Label
    {
        anchors.centerIn: parent
        color: '#fff'
        font.pixelSize: 40
        text: 'Tank'
        font.bold: true
        opacity: 0.25
    }
    Rectangle
    {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 10
        width: 35
        height: width
        radius: width/2
        color: '#aaa'
        border.color: '#ddd'
        visible: true //TODO: hide when running
        Text
        {
            anchors.centerIn: parent
            color: '#666'
            font.family: 'Flaticon'
            font.pixelSize: 18
            text: Flaticon.get('ac-dry')
        }
        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked:  drainTankDialog.open()
            onHoveredChanged:
            {
                if (containsMouse) parent.color = Material.color(Material.Green)
                else parent.color = '#aaa'

            }
        }
    }

    BaseDialog
    {
        id: drainTankDialog
        parent: basePage
        title: 'Drain tank'
        message: 'All liquid in the tank will be discharged? Are you sure?'
        onAccepted: console.log("Xả lũ!!!!")
    }
}

