import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtMultimedia 5.12
import '../commons'

BasePage
{
    isHomePage: true
    property var door
    property var recipe
    property var volume
    property var ingredients

    function done(code)
    {
        timeOut.stop()
        hideLoading();
        if (code === 0)
        {
             makingDialog.open()
        }
        else if (code === 1) {back(); showAppError("Door is busy", 5000);}
        else if (code === 2) {back(); showAppError("Ingredients not enough", 5000);}
        else
        {
            back(); showAppError("Cannot process your order", 5000);
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

    Connections
    {
        target: Home
        onOrderStateChanged:
        {
            console.log("State: "+ state)

            makingDialog.close()
            if (state === 0)
            {
                doneDialog.open()
            }
            else if (state === 1)
            {
                showPageError("Make error", 13000)
                back()
                back()
            }
        }
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        Item
        {
            Layout.fillWidth: true
            Layout.topMargin: -10
            Layout.preferredHeight: 45

            RowLayout
            {
                anchors.fill: parent

                RoundButton
                {
                    Material.theme: Material.Light
                    text: Flaticon.get('back')
                    font.family: 'Flaticon'
                    flat: true
                    onClicked: back()
                }
                Item
                {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 50
                }
            }
        }

        ColumnLayout
        {
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Label
            {
                Layout.fillWidth: true
                text: 'Please put your cup into selected door<br>then press <b>Confirm</b> button below'
                color: '#444'
                wrapMode: Label.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 400
                Image
                {
                    anchors.fill: parent
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/put_cup.png'
                    sourceSize: Qt.size(parent.width, parent.height)
                }

            }
            RoundButton
            {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 150
                Material.accent: Material.Blue
                Material.elevation: 0
                highlighted: true
                text: 'Confirm'
                font.bold: true
                onClicked:
                {
//                    putCupDialog.close()
                    showLoading()
                    var op = Engine.createOperation("machine.MakeDrink")
                    op.finished.connect(done)
                    op.door = door;
                    op.volume = volume;
                    op.ingredients = ingredients;
                    op.fire();
                    timeOut.start();
                }
            }
        }
    }

//    Popup
//    {
//        id: putCupDialog
//        x: parent.x
//        y: parent.height- height
//        implicitWidth: parent.width
//        //        height: parent.height - 200
//        modal: true
//        closePolicy: Dialog.NoAutoClose
//        contentItem: ColumnLayout
//        {
//            width: parent.width -20
//            spacing: 0
//            RoundButton
//            {
//                Layout.alignment: Qt.AlignTop| Qt.AlignRight
//                Layout.topMargin: -10
//                text: Flaticon.get('delete')
//                font.family: 'Flaticon'
//                flat: true
//                onClicked: putCupDialog.close()
//            }
//            Label
//            {
//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: makingDialog.width - 30
//                text: 'Please put your cup into selected door<br>then press <b>Confirm</b> button below'
//                color: '#444'
//                wrapMode: Label.WordWrap
//                horizontalAlignment: Text.AlignHCenter
//            }
//            Item
//            {
//                Layout.fillWidth: true
//                Layout.preferredHeight: 400
//                Image
//                {
//                    anchors.fill: parent
//                    asynchronous: true
//                    fillMode: Image.PreserveAspectFit
//                    source: 'qrc:/images/put_cup.png'
//                    sourceSize: Qt.size(parent.width, parent.height)
//                }

//            }
//            RoundButton
//            {
//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: 150
//                Material.accent: Material.Blue
//                Material.elevation: 0
//                highlighted: true
//                text: 'Confirm'
//                font.bold: true
//                onClicked: putCupDialog.close()
//            }
//        }
//    }

    Popup
    {
        id: makingDialog
        anchors.centerIn: parent
        implicitWidth: parent.width - 20
        height: 175
        modal: true
        closePolicy: Dialog.NoAutoClose
        contentItem: ColumnLayout
        {
            spacing: 0
            Label
            {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: makingDialog.width - 30
                text: 'Brain Juice Machine is processing your order ...\nPlease wait in a second'
                color: '#444'
                wrapMode: Label.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
            LoadingIndicator
            {
                Layout.alignment: Qt.AlignHCenter
                ringSize: 45
                nutSize: 10
                running: true
            }
        }
    }

    Popup
    {
        id: doneDialog
        anchors.centerIn: parent
        implicitWidth: parent.width - 20
        height: 175
        modal: true
        contentItem: ColumnLayout
        {
            spacing: 10
            Label
            {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: makingDialog.width - 30
                text: 'Your drink is ready now\nThank you!!!'
                color: '#444'
                wrapMode: Label.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
            Rectangle
            {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 75
                Layout.preferredHeight: width
                radius: width/2
                color: 'green'
                Label
                {
                    anchors.centerIn: parent
                    text: Flaticon.get('tick')
                    font.family: 'Flaticon'
                    font.pixelSize: 30
                    color: 'white'
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: home()
                }
            }
        }
        onClosed: home()
    }
}
