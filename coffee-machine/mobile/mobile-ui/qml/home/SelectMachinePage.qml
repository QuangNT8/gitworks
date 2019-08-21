import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtMultimedia 5.12
import QtGraphicalEffects 1.0
import QZXing 2.3
import '../commons'

BasePage
{
    id: basePage
    property var door
    property var recipe
    property var volume
    property var ingredients
    isHomePage: true
    headerTitle: 'Select machine'

    property bool isReal: true

    function done(code)
    {
        timeOut.stop()
        hideLoading();
        if (code === 0)
        {
            showPage('qrc:/home/GetDrinkPage.qml', {door: door, volume: volume, ingredients: ingredients, recipe: recipe})
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

    Camera
    {
        id:camera
        focus.focusMode: CameraFocus.FocusContinuous
        focus.focusPointMode: CameraFocus.FocusPointAuto
    }

    background: VideoOutput
    {
        id: videoOutput
        source: camera
        anchors.fill: parent
        autoOrientation: true
        fillMode: VideoOutput.PreserveAspectCrop
        filters: [ zxingFilter ]
    }

    QZXingFilter
    {
        id: zxingFilter
        captureRect:
        {
            videoOutput.contentRect;
            videoOutput.sourceRect;
            return videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(
                                                                                       0.25, 0.25, 0.25, 0.5
                                                                                       )));
        }
        decoder.enabledDecoders: QZXing.DecoderFormat_QR_CODE
        decoder.tryHarder: true
        decoder.onTagFound:
        {
            console.log("onTagFound: " + tag)

            if (tag == "BRAINJUICE01" || tag == "BRAINJUICE02")
            {
                if (isReal)
                {
                    door = tag
                    showLoading()
                    var op = Engine.createOperation("machine.CheckOrder")
                    op.finished.connect(done)
                    op.door = tag;
                    op.volume = volume;
                    op.ingredients = ingredients;
                    op.fire();
                    timeOut.start();
                    isReal = false
                }
            } else
            {
                showPageError("Qr code invalid characters in the text", 5000)
            }
        }

        onDecodingStarted:
        {
            //console.log("onDecodingStarted")
        }

        onDecodingFinished:
        {
            //console.log("onDecodingFinished")
        }
    }

    Item
    {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        height: 45
        z: 10
        RowLayout
        {
            anchors.fill: parent
            RoundButton
            {
                Material.theme: Material.Dark
                text: Flaticon.get('back')
                font.family: 'Flaticon'
                flat: true
                onClicked: back()
            }
            Item
            {
                Layout.fillWidth: true
            }
            RoundButton
            {
                Material.theme: Material.Dark
                text: Flaticon.get('help-bold')
                font.family: 'Flaticon'
                font.pixelSize: 20
                flat: true
                onClicked: guidelineDialog.open()
            }
        }
        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            y: captureZone.y - 40
            width: parent.width - header.height
            text: 'Place the QR code inside the frame'
            color: 'white'
            wrapMode: "WordWrap"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Item
    {
        id: maskZone
        anchors.fill: parent
        Rectangle
        {
            id: rec
            anchors.fill: parent
            visible: false
            color: '#111'
        }
        Rectangle
        {
            id: mask
            anchors.fill: parent
            color: 'transparent'
            visible: false
            Rectangle
            {
                id: captureZone
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height/4
                width: parent.width/2
                height: width
                color: '#333'

            }
        }
        OpacityMask
        {
            anchors.fill: parent
            source: rec
            maskSource: mask
            invert: true
            opacity: 0.5
        }
        Item
        {
            id: scaningVisualEffect
            x: captureZone.x
            y: captureZone.y
            width: captureZone.width
            height: captureZone.height
            Rectangle
            {
                id: scanIndicator
                x: 0
                y: 0
                width: parent.width
                height: 3
                color: 'green'
                opacity: 0.75
                gradient: Gradient
                {
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: "green" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
                Behavior on y
                {
                    NumberAnimation{duration: 1500}
                }
                Timer
                {
                    id: scanIndicatorTimer
                    interval: 100
                    repeat: true
                    onTriggered:
                    {
                        if (scanIndicator.y === 0) scanIndicator.y = scanIndicator.parent.height
                        else if (scanIndicator.y === scanIndicator.parent.height) scanIndicator.y = 0
                    }
                }
                Component.onCompleted: scanIndicatorTimer.start()
            }
        }
    }
    Dialog
    {
        id: guidelineDialog
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        width: parent.width - 10
        title: "How to select machine"
        contentItem: ColumnLayout
        {
            width: parent.width
            spacing: 0
            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                Image
                {
                    anchors.fill: parent
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/images/qrcode_guide.png'
                    sourceSize: Qt.size(parent.width, parent.height)
                }
            }
            RoundButton
            {
                Layout.alignment: Qt.AlignHCenter
                Material.accent: Material.Grey
                Material.elevation: 0
                Layout.preferredWidth: 150
                highlighted: true
                text: 'OK'
                font.bold: true
                onClicked: guidelineDialog.close()
            }
        }
    }
}
