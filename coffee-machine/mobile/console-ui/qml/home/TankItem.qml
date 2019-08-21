import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import '../commons'

Rectangle
{
    Layout.fillWidth: true
    Layout.preferredHeight: width
    Layout.maximumHeight: 300//FIXME
    color:  '#ccc'
    radius: 3
    enabled: model.pouring
    MouseArea
    {
        anchors.fill: parent
        onClicked: editTankDialog.open()
        cursorShape: !model.pouring ? Qt.ForbiddenCursor : Qt.CrossCursor
    }

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

    function openValve()
    {
        if (rbtTime.checked)
        {
            if (openTimeInput.text.length === 0 || openTimeInput.text < 1)
                showPageError('Please check input value', 3000)
            else
            {
                var opTime = Engine.createOperation("machine.OpenValve")
                opTime.index = model.index;
                opTime.time = openTimeInput.text;
                opTime.vol = 0;
                opTime.fire();
            }
        }
        else
        {
            if (openVolumeInput.text.length === 0 || openVolumeInput.text < 1 || openVolumeInput.text > 2000)
                showPageError('Please check input value', 3000)
            else
            {
                var opVol = Engine.createOperation("machine.OpenValve")
                opVol.index = index;
                opVol.time = 0;
                opVol.vol = openVolumeInput.text;
                opVol.fire();
            }
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
            anchors.leftMargin:  10
            anchors.rightMargin: 10

            Label
            {
                Layout.fillWidth: true
                text: Console.getLiquiNamedById(model.liquidid)
                color: '#eee'
                font.pixelSize: 18
                font.bold: true
            }
            Label
            {
                text: !model.pouring ? 'Pouring...' : (model.volume + ' ml')
                font.bold: true
                color: !model.pouring ? Material.color(Material.Pink) : '#ccc'
                font.pixelSize: 14
                visible: model.liquidid !== 0
            }
        }
    }

    Rectangle
    {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        width: parent.width
        color: Material.color(Material.LightGreen)
        height: Math.round((model.volume/2000 * (parent.height-45)))
        Behavior on height {
            NumberAnimation { duration: 750 }
        }
    }

    Label
    {
        anchors.centerIn: parent
        color: '#fff'
        font.pixelSize: 40
        text: '#' + (model.index+1)
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
        color: !model.pouring ? Material.color(Material.Pink) : '#aaa'
        enabled: model.pouring
        border.color: '#ddd'
        visible: Console.debugMode
        Text
        {
            anchors.centerIn: parent
            color: model.pouring ? 'white' : '#666'
            font.family: 'Flaticon'
            font.pixelSize: 18
            text: Flaticon.get('ac-dry')
        }
        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: !model.pouring ? Qt.ForbiddenCursor : Qt.PointingHandCursor
            onClicked: openValveDialog.open()
            onHoveredChanged:
            {
                if (containsMouse) parent.color = Material.color(Material.Green)
                else parent.color = '#aaa'

            }
        }
    }

    BaseDialog
    {
        id: editTankDialog
        parent: home
        title: 'Update tank #' + (index+1) + ' informations'
        height: contentHeight + 150
        onOpened:
        {
            liquidType.model = Console.getLiquids()
            var name = Console.getLiquiNamedById(model.liquidid)
            liquidType.currentIndex = liquidType.find(name)
            tankVolumeInput.forceActiveFocus()
        }

        contentItem: ColumnLayout
        {
            width: parent.width
            spacing: 5
            RowLayout
            {
                spacing: 10
                Label
                {
                    Layout.preferredWidth: 150
                    text: 'Select liquid'
                }
                ComboBox
                {
                    id: liquidType
                    Layout.preferredWidth: 200
                }
            }
            RowLayout
            {
                spacing: 10
                Label
                {
                    Layout.preferredWidth: 150
                    text: 'Enter tank volume'
                }
                TextField
                {
                    id: tankVolumeInput
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 40
                    text: model.volume
                    placeholderText: '100~2000'
                    topPadding: 5
                    bottomPadding: 5
                    rightPadding: 10
                    leftPadding: 10
                    selectByMouse: true
                    validator: IntValidator{bottom: 0; top: 2000}
                    inputMethodHints: Qt.ImhDigitsOnly
                    background: Rectangle{border.color: '#aaa'; radius: 3}
                    onTextChanged: if (parseInt(text, 10) > 2000) text = '2000'
                    onFocusChanged: if (focus) selectAll()
                }
                Label
                {
                    text: 'ml'
                }
            }
        }
        onAccepted:
        {
            editTank()
            liquidType.currentIndex = -1
        }
        onRejected:
        {
            liquidType.currentIndex = -1
        }
    }

    BaseDialog
    {
        id: openValveDialog
        parent: home
        height: contentHeight + 150
        title: 'Open tank #' + (index+1) + ' ' + Console.getLiquiNamedById(model.liquidid)
        contentItem: ColumnLayout
        {
            width: parent.width
            spacing: 5
            ButtonGroup
            {
                id: valveOpenType
            }
            RowLayout
            {
                spacing: 5
                RadioButton
                {
                    id: rbtVolume
                    Layout.preferredWidth: 125
                    ButtonGroup.group: valveOpenType
                    text: 'By volume'
                    checked: model.volume !== 0
                    enabled: model.volume !== 0
                    onPressed: openVolumeInput.forceActiveFocus()
                }
                TextField
                {
                    id: openVolumeInput
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: '200'
                    topPadding: 5
                    bottomPadding: 5
                    rightPadding: 10
                    leftPadding: 10
                    validator: IntValidator{bottom: 0; top: 1000}
                    background: Rectangle{border.color: '#aaa'; radius: 3}
                    inputMethodHints: Qt.ImhDigitsOnly
                    enabled: rbtVolume.checked
                    onFocusChanged: if (focus) selectAll()
                }
                Label
                {
                    text: 'ml'
                }
            }
            RowLayout
            {
                spacing: 5
                RadioButton
                {
                    id: rbtTime
                    Layout.preferredWidth: 125
                    ButtonGroup.group: valveOpenType
                    text: 'In time'
                    checked: model.volume === 0
                    onPressed: openVolumeInput.forceActiveFocus()
                }
                TextField
                {
                    id: openTimeInput
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: '5000'
                    topPadding: 5
                    bottomPadding: 5
                    rightPadding: 10
                    leftPadding: 10
                    validator: IntValidator{bottom: 0; top: 10000}
                    background: Rectangle{border.color: '#aaa'; radius: 3}
                    enabled: rbtTime.checked
                    onFocusChanged: if (focus) selectAll()
                }
                Label
                {
                    text: 'ms'
                }
            }
        }
        onAccepted: openValve()
    }
}

