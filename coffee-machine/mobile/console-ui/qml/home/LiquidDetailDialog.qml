import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.3
import '../commons'

Popup
{
    property var currentLiquid: null
    property var trainingFile: ""

    function done(code)
    {
        timeOut.stop()
        hideLoading();
        if (code !== 0) showPageError("Cannot update liquid", 3000)
        else
        {
            trainingFile = ""
            detailDialog.close()
        }
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

    function addLiquid()
    {
        if (liquidNameEdit.text.length===0) showPageError('Please enter new liquid name', 3000)
        else if (trainingFile === "") showPageError("Please select file data training", 3000)
        else
        {
            showLoading()
            var op = Engine.createOperation("machine.UpdateLiquid")
            op.finished.connect(done)
            op.model = null;
            op.name = liquidNameEdit.text;
            op.file = trainingFile;
            op.fire();
        }
    }

    function updateLiquid()
    {
        if (liquidNameEdit.text.length === 0) showPageError('Please enter new liquid name', 3000)
        else
        {
            showLoading()
            var op = Engine.createOperation("machine.UpdateLiquid")
            op.finished.connect(done)
            op.model = currentLiquid;
            op.name = liquidNameEdit.text;
            op.file = trainingFile;
            op.fire();
        }
    }

    function clearNetwork()
    {
        txtLayers.text = "";
        txtWeights.text = "";
        txtMin.text = "";
        txtMax.text = "";
    }

    id: detailDialog
    anchors.centerIn: parent
    implicitWidth: 680
    implicitHeight: 600
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose
    contentItem: ColumnLayout
    {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 0
        spacing: 5
        SectionTitle
        {
            Layout.rightMargin: 10
            text: 'Name'
            icon: ''
        }
        RectangularInput
        {
            id: liquidNameEdit
            Layout.fillWidth: true
            text: currentLiquid === null ? "New liquid" : currentLiquid.name
            maximumLength: 50
        }
        SectionTitle
        {
            Layout.rightMargin: 10
            text: 'Neural Network'
            icon: Flaticon.get('folder')
            onClicked: trainingFileDialog.open()
        }

        NetworkGraphItem
        {
            id: networkGraphItem
            //TODO: impove this
        }

        RowLayout
        {
            id: buttonLayout
            spacing: 15
            Button
            {
                Layout.preferredWidth: 100
                text: 'Delete'
                flat: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                visible: /*currentLiquid !== null*/ false
                onClicked:
                {
                    //TODO
                }
            }
            Item
            {
                Layout.fillWidth: true
            }
            Button
            {
                Layout.preferredWidth: 100
                text: 'Cancel'
                flat: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                onClicked:
                {
                    detailDialog.close()
                }
            }
            Button
            {
                Layout.preferredWidth: 100
                Material.accent: Material.color(Material.Blue)
                text: 'Save'
                flat: true
                highlighted: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                onClicked:
                {
                    if (currentLiquid === null) addLiquid();
                    else updateLiquid();
                }
            }
        }
    }
    FileDialog
    {
        id: trainingFileDialog
        title: "Please choose a image file"
        folder: shortcuts.home
        onAccepted: trainingFile= trainingFileDialog.fileUrl
    }
}
