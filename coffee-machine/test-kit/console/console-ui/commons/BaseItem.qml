import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0

Pane
{
    id: baseItem
    signal clicked()
    property alias color : recBack.color
    property bool favorite: false

    background: Item
    {
        RectangularGlow
        {
            id: effect
            anchors.fill: recBack
            anchors.topMargin: 2
            anchors.leftMargin: 2
            anchors.rightMargin: 2
            glowRadius: 3
            spread: 0.1
            color: "#aaa"
            cornerRadius: recBack.radius + glowRadius
        }
        Rectangle
        {
            id: recBack
            anchors.fill: parent
            clip: true
            radius: 5
            color:'#fff'
        }

        Item
        {
            visible: favorite
            anchors.fill: recBack
            layer.enabled: true
            layer.effect: OpacityMask
            {
                maskSource: recBack
            }

            Rectangle
            {
                anchors.top: parent.top
                anchors.left: parent.left
//                anchors.topMargin: - width/2 - 7
//                anchors.leftMargin: - width/2 - 7
                anchors.topMargin: - width/2 - baseItem.topPadding/2 //Test to fix scale on multi device
                anchors.leftMargin: - width/2 - baseItem.leftPadding/2
                //color:"#ccc"
                color: Material.color(Material.Orange)
                width: 60
                height: width
                radius: width/2
                Text
                {
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
//                    anchors.bottomMargin: 8
//                    anchors.rightMargin: 8
                    anchors.bottomMargin: parent.height/8 + height/8//Test to fix scale on multi device
                    anchors.rightMargin: parent.width/8 + width/8
                    color: '#fff'//Material.color(Material.Yellow)
                    font.bold: true
                    font.pixelSize: 9
                    font.family: 'flaticon'
                    text: Flaticon.get('star-black')
                }
            }
        }
        MouseArea
        {
            anchors.fill: parent
            anchors.margins: -baseItem.padding
            //FIXME: since Pane have paddings by itself, this margin value make the MouseArea get correct visual working zone
            onClicked: baseItem.clicked()
        }
    }
    Item
    {
        /*TODO: remove me if possible*/
    }
}
