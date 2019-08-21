import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.3
import'../commons'

BasePage
{
    id: basePage
    isHomePage: true
    property bool showLog: false
    function addProject()
    {
        if (projectNameInput.text.length===0) showPageError('Please enter new project name', 3000)
        else if (sampleTotalInput.text.length === 0) showPageError("Please enter sample total", 3000)
        else
        {
            showLoading()
            var op = Engine.createOperation("kit.CreateProject")
            op.finished.connect(done)
            op.name = projectNameInput.text;
            op.total = sampleTotalInput.text;
            op.tankSize = tankSizeInput.text;
            op.fire();
            timeOut.start()
        }
    }
    function cleanDone(code)
    {
        timeOut.stop()
        hideLoading();
        if (code !== 0) showPageError("Cannot clean tank", 3000)
    }

    function stopDone(code)
    {
        timeOut.stop()
        hideLoading();
        if (code !== 0) showPageError("Cannot stop", 3000)
    }

    function done(code)
    {
        timeOut.stop()
        hideLoading();
        if (code !== 0) showPageError("Cannot create project", 3000)
        else
        {
            sampleTotalInput.text = 10000
            projectNameInput.text = ""
        }
    }
    function openValeByTime()
    {
        if (openVale.text.length===0) showPageError('Please enter time open', 3000)
        else
        {
            showLoading()
            var op = Engine.createOperation("kit.OpenValue")
            op.finished.connect(doneManual)
            op.value = openVale.text;
            op.project = null;
            op.fire();
            timeOut.start()
        }
    }
    function doneManual(code)
    {
        timeOut.stop()
        hideLoading();
        if (code !== 0) showPageError("Cannot process", 3000)
    }
    function refill()
    {

        showLoading()
        var op = Engine.createOperation("kit.ReFillTank")
        op.finished.connect(doneManual)
        op.fire();
        timeOut.start()

    }
    function flushTank()
    {

        showLoading()
        var op = Engine.createOperation("kit.FlushTank")
        op.finished.connect(doneManual)
        op.fire();
        timeOut.start()

    }
    Timer
    {
        id: timeOut
        interval: 15000; running: false; repeat: false
        onTriggered:
        {
            showPageError("The connection to the server failed.")
            hideLoading();
        }
    }

    RowLayout
    {
        anchors.fill: parent
        spacing: 5
        Rectangle
        {
            id: projectRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: '#eee'
            ColumnLayout
            {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                z: 10
                Item
                {
                    Layout.fillHeight: true
                }
                RoundButton
                {
                    id: btnClean
                    z: 10
                    Material.accent: Kit.state === 2 ? Material.Orange : Material.Blue
                    Material.elevation: 0
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: width
                    radius: width/2
                    text: Kit.state === 2 ? 'Stop' : 'Clean\nkit'
                    font.pixelSize: Kit.state === 2 ? 10 : 13
                    font.bold: true
                    highlighted: true
                    onClicked:
                    {
                        if (Kit.state === 0)
                        {
                            showLoading()
                            var clean = Engine.createOperation("kit.CleanTank")
                            clean.finished.connect(cleanDone)
                            clean.fire();
                            timeOut.start()
                        }
                        else if (Kit.state === 2)
                        {
                            showLoading()
                            var stop = Engine.createOperation("kit.StopProject")
                            stop.finished.connect(stopDone)
                            stop.fire();
                            timeOut.start()
                        }
                        else
                        {
                            showPageError("Kit is running!", 5000)
                        }

                    }
                }

                RoundButton
                {
                    id: btnValve
                    Material.accent: Material.Red
                    Material.elevation: 0
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: width
                    radius: width/2
                    text: 'Valve'
                    font.bold: true
                    highlighted: true
                    onClicked: openValveDialog.open()
                }
                BaseDialog
                {
                    id: openValveDialog
                    parent: basePage
                    title: "Open valve"
                    TextField
                    {
                        id: openVale
                        width: parent.width
                        text: '1000'
                        validator: RegExpValidator{regExp: /^[0-9]*$/}
                    }
                    onAccepted: openValeByTime()
                }
                RoundButton
                {
                    id: btnReFill
                    Material.accent: Material.Red
                    Material.elevation: 0
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: width
                    radius: width/2
                    text: 'ReFill'
                    font.bold: true
                    highlighted: true
                    onClicked: refill()
                }
                RoundButton
                {
                    Material.accent: Material.Red
                    Material.elevation: 0
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: width
                    radius: width/2
                    text: 'Flush'
                    font.bold: true
                    highlighted: true
                    onClicked: flushTank()
                }
                RoundButton
                {
                    Material.accent: Material.Indigo
                    Material.elevation: 0
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: width
                    radius: width/2
                    text: showLog ? 'Hide\nLog' : 'Show\nLog'
                    font.bold: true
                    highlighted: true
                    onClicked: showLog = !showLog
                }
            }
            Flickable
            {
                anchors.fill: parent
                anchors.margins: 10
                contentWidth: width
                contentHeight: grid.height
                GridLayout
                {
                    id: grid
                    width: parent.width
                    columns: 5
                    columnSpacing: 5
                    rowSpacing: 5
                    Rectangle
                    {
                        Layout.preferredWidth: (parent.width-(grid.columns-1)*grid.columnSpacing)/grid.columns
                        Layout.preferredHeight: width/2
                        color: '#ccc'
                        border.color: '#ccc'
                        radius: 3
                        Label
                        {
                            anchors.centerIn: parent
                            text: Flaticon.get('add')
                            font.family: 'Flaticon'
                            font.pixelSize: 35
                            color: '#666'
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onPressed: parent.color = '#bbb'
                            onReleased: parent.color = '#ccc'
                            onClicked: createProjectDialog.open()
                        }
                    }
                    Repeater
                    {
                        id: repeater
                        model: Kit.projects
                        delegate: Rectangle
                        {
                            Layout.preferredWidth: (parent.width-(grid.columns-1)*grid.columnSpacing)/grid.columns
                            Layout.preferredHeight: width/2
                            color: '#ccc'
                            radius: 3
                            Image
                            {
                                anchors.fill: parent
                                anchors.margins: 10
                                asynchronous: true
                                fillMode: Image.PreserveAspectFit
                                source: ''
                                opacity: 0.25
                            }
                            ColumnLayout
                            {
                                anchors.centerIn: parent
                                width: parent.width - 20
                                Label
                                {
                                    Layout.fillWidth: true
                                    text: model.name
                                    color: '#444'
                                    font.pixelSize: 16
                                    font.bold: true
                                    maximumLineCount: 2
                                    wrapMode: Label.Wrap
                                    elide: "ElideRight"
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                Label
                                {
                                    Layout.fillWidth: true
                                    text: Kit.convertTimeByLong(model.date)
                                    color: '#666'
                                    font.pixelSize: 13
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked:
                                {
                                    projectDetail.project = model
                                    projectDetail.active = true
                                }
                            }
                            Label
                            {
                                id: runningLabel
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                anchors.rightMargin: 5
                                anchors.bottomMargin: 5
                                text: "Running"
                                color: 'white'
                                font.pixelSize: 11
                                font.capitalization: Font.AllUppercase
                                topPadding: 3
                                bottomPadding: 3
                                rightPadding: 6
                                leftPadding: 6
                                background: Rectangle
                                {
                                    color: Material.color(Material.Green)
                                    radius: 2
                                }
                                visible:
                                {
                                    if (Kit.currentId === model.id)
                                    {
                                        if (Kit.state === 3 || Kit.state === 1) return true;
                                        else return false;
                                    }
                                    else return false;
                                }
                            }
                            Label
                            {
                                id: buildingLabel
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                anchors.rightMargin: 5
                                anchors.bottomMargin: 5
                                text: "Building"
                                color: 'white'
                                font.pixelSize: 11
                                font.capitalization: Font.AllUppercase
                                topPadding: 3
                                bottomPadding: 3
                                rightPadding: 6
                                leftPadding: 6
                                background: Rectangle
                                {
                                    color: Material.color(Material.Indigo)
                                    radius: 2
                                }
                                visible:
                                {
                                    if (Kit.currentId === model.id)
                                    {
                                        if (Kit.state === 4) return true;
                                        else return false;
                                    }
                                    else return false;
                                }
                            }
                        }
                    }
                    Item
                    {
                        Layout.fillWidth: true
                        visible: repeater.count < grid.columns
                    }
                }

                BaseDialog
                {
                    id: createProjectDialog
                    title: 'Create new project'
                    height: contentHeight + 150
                    contentItem: ColumnLayout
                    {
                        width: parent.width
                        spacing: 10
                        RowLayout
                        {
                            Layout.fillWidth: true
                            spacing: 15
                            Label
                            {
                                Layout.preferredWidth: 120
                                text: 'Project name'
                                font.pixelSize: 15
                                color: '#333'
                            }
                            TextField
                            {
                                id:projectNameInput
                                Layout.fillWidth: true
                                text: "New project"
                                placeholderText: 'Enter project name'
                                maximumLength: 50
                            }
                        }
                        RowLayout
                        {
                            Layout.fillWidth: true
                            spacing: 15
                            Label
                            {
                                Layout.preferredWidth: 120
                                text: 'Sample size'
                                font.pixelSize: 15
                                color: '#333'
                            }
                            TextField
                            {
                                id: sampleTotalInput
                                Layout.preferredWidth: 150
                                Layout.preferredHeight: 40
                                text: "10000"
                                topPadding: 5
                                bottomPadding: 5
                                rightPadding: 10
                                leftPadding: 10
                                selectByMouse: true
                                validator: RegExpValidator{regExp: /^[0-9]*$/}
                                inputMethodHints: Qt.ImhDigitsOnly
                                background: Rectangle{border.color: '#aaa'; radius: 3}
                                onFocusChanged: if (focus) selectAll()
                            }
                        }
                        RowLayout
                        {
                            Layout.fillWidth: true
                            spacing: 15
                            Label
                            {
                                Layout.preferredWidth: 120
                                text: 'Tank size (ml)'
                                font.pixelSize: 15
                                color: '#333'
                            }
                            TextField
                            {
                                id: tankSizeInput
                                Layout.preferredWidth: 150
                                Layout.preferredHeight: 40
                                text: "2000"
                                topPadding: 5
                                bottomPadding: 5
                                rightPadding: 10
                                leftPadding: 10
                                selectByMouse: true
                                validator: RegExpValidator{regExp: /^[0-9]*$/}
                                inputMethodHints: Qt.ImhDigitsOnly
                                background: Rectangle{border.color: '#aaa'; radius: 3}
                                onFocusChanged: if (focus) selectAll()
                                enabled: false
                            }
                        }

                    }
                    onAccepted: addProject()
                    onRejected:
                    {
                        sampleTotalInput.text = 10000
                        projectNameInput.text = ""
                    }
                }

            }

            ProjectDetailDialog
            {
                id: projectDetail
            }

        }
        Rectangle
        {
            id: logRect
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            color: '#333'
            visible: showLog
            Loader
            {
                anchors.fill: parent
                anchors.margins: 10
                active: showLog
                source: "qrc:/home/LogView.qml"
            }
        }
    }
}
