import QtQuick 2.12
import QtQuick.Controls 2.5

//Rectangle//style1
//{
//    anchors.top: parent.top
//    anchors.right: parent.right
//    width: 32
//    height: width
//    anchors.topMargin: -height/2 - 11
//    anchors.rightMargin: -width/2 - 11
//    color: '#aaa'
//    transform: Rotation { origin.x: 16; origin.y: 16; angle: 45}
//    Label
//    {
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        anchors.bottomMargin: parent.height/8 - height/4
//        anchors.leftMargin: parent.width/2 - width/2
//        text: Flaticon.get('star-black')
//        font.family: 'Flaticon'
//        font.pixelSize: 8
//        color: '#eee'
////            transform: Rotation { origin.x: 4; origin.y: 4; angle: -45}
//    }
//}

Rectangle //style2
{
    anchors.top: parent.top
    anchors.right: parent.right
    width: 64
    height: 10
    color: '#aaa'
    transform: Rotation { origin.x: 54; origin.y: 54; angle: 45}
    Label
    {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: parent.height/ - height/2
        anchors.leftMargin: 19
        text: Flaticon.get('star-black')
        font.family: 'Flaticon'
        font.pixelSize: 8
        color: '#eee'
//            transform: Rotation { origin.x: 4; origin.y: 4; angle: -45}
    }
}

