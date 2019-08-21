import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0

Item
{
    id: entityHeader
    property alias rightItem: rightLoader.sourceComponent
    property alias leftItem: leftLoader.sourceComponent
    property alias actionBar: barLoader.sourceComponent

    RectangularGlow
    {
        id: effect
        anchors.fill: recBack
        anchors.topMargin: 3
        anchors.leftMargin: 2
        anchors.rightMargin: 1
        glowRadius: 3
        spread: 0.1
        color: "#444"
        cornerRadius: recBack.radius + glowRadius
    }

    Rectangle
    {
        clip: true
        radius: 3
        anchors.fill: parent
        id: recBack
        color:'#666'
    }

    Item
    {
        anchors.fill: recBack
        layer.enabled: true
        layer.effect: OpacityMask
        {
            maskSource: recBack
        }
        Rectangle
        {
            anchors.fill: parent
            anchors.topMargin: parent.height - 40
            color:"#888"
            Loader
            {
                id: barLoader
                anchors.centerIn: parent
                height: parent.height
            }
        }
    }

    RowLayout
    {
        id: header
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10
        anchors.bottomMargin: 50

        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Loader
            {
                anchors.fill: parent
                id: leftLoader
            }
        }

        Rectangle
        {
            width : 1
            Layout.fillHeight: true
            color: '#888'
        }

        Item
        {

            Layout.fillWidth: true
            Layout.fillHeight: true
            Loader
            {
                anchors.fill: parent
                id: rightLoader
            }
        }
    }
}

