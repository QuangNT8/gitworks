import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.4
import QtQuick.Controls.Material 2.3
import '../commons'

BasePage
{
    property bool machineReady: Console.machineReady
    property int lineLength: Console.getLineLength()
    property int minDistanceTwoRobot: Console.getMinDistanceTwoRobot()
    property int maxPositon: Console.getMaxPositionRoBot()
    isHomePage: true
    spacing: 5

//    onDebugModeChanged:
//    {
//        sendLogWarning("Debug Mode changed: " + debugMode)
//    }
    function debugDone(code)
    {
        hideLoading()
        timeOut.stop()
        if (code !== 0)
        {
            showPageError("Cannot change debugmode. Please try again!", 3000)
        }

    }

    Timer
    {
        id: timeOut
        interval: 3000; running: false; repeat: false
        onTriggered:
        {
            showPageError("The connection to the server failed.")
            hideLoading()
        }
    }

    ColumnLayout
    {
        id: machineTabLayout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5
        visible: machineReady
        enabled: Console.debugMode
        SectionTitle
        {
            text: 'Cups'
            icon: ''
        }
        Pane
        {
            Layout.fillWidth: true
            background: Rectangle{color: '#ddd'; radius: 3}
            ColumnLayout
            {
                anchors.fill: parent
                spacing: 0

                MotorSlider
                {
                    id:motor
                    property int lastValueFirst: Console.leftMotor.position
                    property int lastValueSecond: lineLength - Console.rightMotor.position
                    Layout.fillWidth: true
                    first.value: lastValueFirst
                    second.value: lastValueSecond
                    from: 0
                    to: lineLength
                    stepSize: 1
                    snapMode: "SnapOnRelease"
                    first.onPressedChanged:
                    {
                        if (!first.pressed && first.value !==lastValueFirst)
                        {
                            var value = first.value;
                            //check 1;
                            if (value >= second.value)
                            {
                                value = second.value - minDistanceTwoRobot;
                            }
                            //check 2;
                            if (second.value - value < minDistanceTwoRobot)
                            {
                                value = second.value - minDistanceTwoRobot;
                            }
                            //check 3;
                            if (value > maxPositon)
                            {
                                value = maxPositon;
                            }

                            first.value = lastValueFirst;
                            var op = Engine.createOperation("machine.MoveTo")
                            op.index = 1; //MOTOR_LEFT
                            op.position = value;
                            op.fire();
                        }
                    }
                    second.onPressedChanged:
                    {
                        if (!second.pressed && second.value !== lastValueSecond)
                        {
                            var value = second.value;
                            //check 1;
                            if (value <= first.value)
                            {
                                value = first.value + minDistanceTwoRobot;
                            }
                            //check 2;
                            if (value - first.value < minDistanceTwoRobot)
                            {
                                value = first.value + minDistanceTwoRobot;
                            }
                            //check 3;
                            if ((lineLength - value) > maxPositon)
                            {
                                value = lineLength - maxPositon;
                            }
                            second.value = lastValueSecond;
                            var op = Engine.createOperation("machine.MoveTo")
                            op.index = 2; //MOTOR_RIGHT
                            op.position = lineLength - value;
                            op.fire();
                        }
                    }
                    Connections
                    {
                        target: Console.leftMotor
                        onPositionChanged:
                        {
                            motor.first.value = motor.lastValueFirst
                        }
                    }
                    Connections
                    {
                        target: Console.rightMotor
                        onPositionChanged:
                        {
                            motor.second.value = motor.lastValueSecond
                        }
                    }
                }
            }
        }

        SectionTitle
        {
            text: 'Tanks'
            icon: ''
        }
        GridLayout
        {
            id: tankSection
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 4
            rows: 2
            columnSpacing: 5
            rowSpacing: 5
            Repeater
            {
                model: Console.tanks
                delegate: TankItem{}
            }
        }

        //        Item
        //        {
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true
        //        }
    }
    LoadingIndicator
    {
        id: machineLoadingIndicator
        anchors.centerIn: parent
        running: !machineReady
    }

    footer: Item
    {
//        color: '#eee'
        height: 35

        RowLayout
        {
            anchors.centerIn: parent
            width: parent.width - 20
            spacing: 10
            Rectangle
            {
                id: machineStateColor
                Layout.preferredWidth: 15
                Layout.preferredHeight: width
                radius: width/2
                color: machineReady ? Material.color(Material.Green) : Material.color(Material.Orange)
            }

            Label
            {
                id: machineStateText
                Layout.fillHeight: true
                text: machineReady ? 'Ready' : 'Waiting'
                font.pixelSize: 13
                color: '#333'
                verticalAlignment: Text.AlignVCenter
            }
            Item
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            Label
            {
                text: 'Debug Mode: ' + (Console.debugMode? 'ON ' : 'OFF')
                font.pixelSize: 12
                color: Console.debugMode ? '#F44336' : '#aaa'
                font.bold: true
                topPadding: 4
                bottomPadding: 4
                leftPadding: 6
                rightPadding: 6
                background: Rectangle
                {
                    color: 'transparent'
                    border.color: Console.debugMode ? '#F44336' : '#aaa'
                    radius: 3
                }
                enabled: machineReady
                MouseArea
                {
                    anchors.fill: parent
                    onDoubleClicked:
                    {
                        showLoading()
                        var op = Engine.createOperation("machine.UpdateDebugMode")
                        op.finished.connect(debugDone)
                        op.value = !Console.debugMode;
                        op.fire();
                        timeOut.start()
                    }
                }                
            }
        }
    }

    Popup
    {
        id: machineRestartingDialog
        anchors.centerIn: parent
        width: 300
        height: 200
        modal: true
        horizontalPadding: 200
        closePolicy: Popup.NoAutoClose | Popup.CloseOnEscape
    }
}

