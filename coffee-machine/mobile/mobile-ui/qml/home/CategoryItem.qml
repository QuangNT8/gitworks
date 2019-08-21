import QtQuick 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

Rectangle
{
    Layout.fillWidth: true
    Layout.preferredHeight: 150
    radius: 5
    color: '#f8f8f8'
    border.color: '#ddd'
    clip: true
    MouseArea
    {
        anchors.fill: parent
        onClicked: showPage('qrc:/home/RecipeDetailPage.qml', {recipe : model, imgSource : img.source})
    }

    Image
    {
        id: img
        anchors.fill: parent
        anchors.bottomMargin: 30
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        sourceSize: Qt.size(parent.width, parent.height)
        layer.enabled: true
        layer.effect: OpacityMask
        {
            maskSource: Item
            {
                width: img.width
                height: img.height
                Rectangle
                {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    radius: 5
                    Rectangle
                    {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: parent.radius
                        color: 'black'//to reject mask effect
                    }
                }
            }
        }
        Component.onCompleted:
        {
            var tmp = model.image.trim();
            if (tmp.startsWith('http')) img.source = tmp;
            else if(tmp.length > 0)
            {
                var f = file.getFilePath(model.image);
                if (f.length > 0)
                {
                    img.source = 'file://'+f
                }
                else
                {
                    var op = Engine.createOperation("machine.DownloadFile")
                    op.finished.connect(fileDownloaded)
                    op.name = model.image;
                    op.fire();
                }
            }
            else
            {
                img.source ='qrc:/images/default_recipe.jpg'
            }

        }

        function fileDownloaded(code)
        {
            if (code === 0)
            {
                img.source = 'file://' + file.location() +'/'+ model.image
            }
            else
            {
                img.source ='qrc:/images/default_recipe.jpg'
            }
        }
    }
    Item
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 30
        Label
        {
            anchors.centerIn: parent
            width: parent.width - 10
            text: model.name
            horizontalAlignment: Text.AlignHCenter
            font.capitalization: Font.Capitalize
            elide: "ElideRight"
        }
    }
}
