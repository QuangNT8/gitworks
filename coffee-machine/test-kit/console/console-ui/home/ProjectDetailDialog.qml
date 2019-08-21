import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.3
import "../commons"

Loader
{
    id: loader
    property var project
    anchors.fill: parent
    active: false
    onLoaded:
    {
        item.project = project
        item.open()
    }
    sourceComponent: Popup
    {
        id: popup
        function removeProject()
        {
            showLoading()
            var op = Engine.createOperation("kit.RemoveProject")
            op.finished.connect(done)
            op.project = project;
            op.fire();
            timeOut.start()
        }
        function done(code)
        {
            timeOut.stop()
            hideLoading();
            loader.active = false
            if (code === 1) showPageError("Project is running! Please stop before delete project", 3000)
        }
        function build(code)
        {
            timeOut.stop()
            hideLoading();
            loader.active = false
            if (code !== 0) showPageError("Cannot build project! Please stop before build project", 3000)
        }
        function start(code)
        {
            timeOut.stop()
            hideLoading();
            loader.active = false
            if (code === 1) showPageError("Other project is running! Please stop before start project", 3000)
            else if (code === 2) showPageError("This project is running! Cannot start project", 3000)
        }

        function openValeByVolum()
        {
            if (openVale.text.length===0) showPageError('Please enter volum open', 3000)
            else
            {
                showLoading()
                var op = Engine.createOperation("kit.OpenValue")
                op.finished.connect(doneManual)
                op.value = openVale.text;
                op.project = project;
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

        property var project
        property int machineState: 0
        anchors.centerIn: parent
        implicitWidth: 750
        implicitHeight: 700
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose
        onClosed: loader.active = false
        onOpened:
        {
            project.samples(chartSection.lineChart)
        }

        contentItem: ColumnLayout
        {
            anchors.fill: parent
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 5
            Label
            {
                Layout.fillWidth: true
                text: project.name
                color: '#333'
                font.pixelSize: 20
                font.bold: true
                elide: Label.ElideRight
            }
            Rectangle
            {
                Layout.fillWidth: true
                Layout.preferredHeight: childrenRect.height + 20
                color: 'transparent'
                border.color: '#ccc'
                radius: 5
                RowLayout
                {
                    anchors.centerIn: parent
                    width: parent.width - 20
                    spacing: 10
                    ColumnLayout
                    {
                        spacing: 5
                        RowLayout
                        {
                            spacing: 10
                            Label
                            {
                                text: 'Samples'
                                color: '#333'
                                font.pixelSize: 13
                                font.capitalization: Font.AllUppercase
                            }
                            Label
                            {
                                text: (project.total-project.remain) + '/' + project.total
                                color: '#666'
                                font.pixelSize: 18
                                font.bold: true
                            }
                        }
                        CustomProgressBar
                        {
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: 100*(project.total-project.remain)/project.total
                        }
                    }
                    Item
                    {
                        Layout.preferredWidth: 25
                    }
                    Button
                    {
                        Layout.preferredWidth: 100
                        Material.accent: Material.color(Material.Indigo)
                        highlighted: true
                        text: 'Build'//TODO: 'Building...'  when building
                        onClicked:
                        {
                            var op = Engine.createOperation("kit.BuildNetwork")
                            op.finished.connect(build)
                            op.project = project;
                            op.fire()
                        }
                    }
                    Button
                    {
                        Layout.preferredWidth: 100
                        Material.accent: Material.color(Material.BlueGrey)
                        highlighted: true
                        text: 'Export' //TODO: ""Exporting..." when exporting
                    }
                }
            }

            ChartSection
            {
                id: chartSection
            }

            RowLayout
            {
                id: buttonSection
                spacing: 15
                Button
                {
                    Layout.preferredWidth: 100
                    Material.accent: Material.color(Material.Red)
                    text: 'Delete'
                    flat: true
                    highlighted: true
                    hoverEnabled: true
                    font.bold: hovered ? true : false
                    onClicked: deleteProjectConfirm.open()
                }
                Item
                {
                    Layout.fillWidth: true
                }
                Button
                {
                    Layout.preferredWidth: 100
                    Material.accent: Material.color(Material.Red)
                    text: "\u25A0 Stop"
                    flat: true
                    highlighted: true
                    hoverEnabled: true
                    font.bold: true
                    visible:
                    {
                        if (Kit.currentId !== project.id) return false;
                        else
                        {
                            if (Kit.state === 2 || Kit.state === 3) return true;
                            else return false;
                        }
                    }

                    onClicked:
                    {
                        var op = Engine.createOperation("kit.StopProject")
                        op.finished.connect(done)
                        op.fire();
                    }
                }
                Button
                {
                    Layout.preferredWidth: 100
                    Material.accent: Material.color(Material.Green)
                    text: "\u25B6 Start"
                    flat: true
                    highlighted: true
                    hoverEnabled: true
                    font.bold: true
                    visible: Kit.state === 0
                    onClicked:
                    {
                        var op = Engine.createOperation("kit.StartProject")
                        op.finished.connect(start)
                        op.project = project;
                        op.fire();
                    }
                }
                Button
                {
                    Layout.preferredWidth: 100
                    Material.accent: Material.color(Material.Red)
                    text: "Open"
                    flat: true
                    highlighted: true
                    hoverEnabled: true
                    font.bold: true
                    onClicked: openValveDialog.open()
                }

                Button
                {
                    Layout.preferredWidth: 100
                    Material.accent: Material.color(Material.Blue)
                    text: "Close"
                    flat: true
                    highlighted: true
                    hoverEnabled: true
                    font.bold: true
                    onClicked: loader.active = false
                }
            }
        }
        BaseDialog
        {
            id: deleteProjectConfirm
            width: parent.width/2
            title: 'Delete project'
            message: 'This can not be undone. Are you sure you want to delete this project?'
            onAccepted: removeProject()
        }
        BaseDialog
        {
            id: openValveDialog
            title: "Open volum (ml)"
            width: parent.width/2
            TextField
            {
                id: openVale
                width: 200
                text: '200'
                validator: RegExpValidator{regExp: /^[0-9]*$/}
            }
            onAccepted: openValeByVolum()
        }
    }
}
