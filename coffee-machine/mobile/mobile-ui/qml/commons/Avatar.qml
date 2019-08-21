import QtQuick 2.12
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Item
{
    width: 70
    height: width
    property string avatar

    function fileDownloaded(id, path)
    {
        try
        {
            img.source = 'file://' + path
        }
        catch (error) {}
    }

    Component.onCompleted:
    {
        if(avatar === '')
        {
            img.source = 'qrc:/images/noavatar.png'
        }
    }

    onAvatarChanged:
    {
        var f = Engine.downloadFile(avatar);
        if (f.ready())
        {
            var tmp = f.path();
            if (tmp.startsWith('http')) img.source = tmp; //FIXME: temporary
            else img.source = 'file://'+tmp
        }
        else
        {
            img.source = 'qrc:/images/noavatar.png'
            f.completed.connect(fileDownloaded)
        }
    }

    Image
    {
        id: img
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        asynchronous: true//only valid for images read from the local filesystem. Images loaded via a network resource (e.g. HTTP) are always loaded asynchronously
        sourceSize.width: parent.width*2
        sourceSize.height: parent.height*2
        layer.enabled: true
        layer.effect: OpacityMask
        {
            maskSource: mask
        }
    }

    Rectangle
    {
        id: mask
        anchors.fill: parent
        radius: width/2
        visible: false
    }
}
