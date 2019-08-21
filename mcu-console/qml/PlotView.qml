import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.2

ChartView
{
	theme: ChartView.ChartThemeHighContrast
    antialiasing: true
    backgroundRoundness: 0
    dropShadowEnabled: false
    //legend.visible: false
    //legend.alignment: Qt.AlignBottom

    function setYRange(min, max)
    {
        if (!isZoomed())
        {
            yAxis.min = min;
            yAxis.max = max;
        }
    }

    function setXRange(min, max)
    {
        if (!isZoomed())
        {
            xAxis.min = min;
            xAxis.max = max;
        }
    }


    ValueAxis
    {
        id: xAxis
        min: 0
        max: 1000
        labelFormat: "%d"
        minorTickCount: 2
        //labelsVisible: false
    }

    ValueAxis
    {
        id: yAxis
        min: 0
        max: 1000
        labelFormat: "%d"
        minorTickCount: 2
    }

    LineSeries
    {
        axisX: xAxis
        axisY: yAxis
        name:"Untitle"
    }

    Rectangle
    {
            id: rectang
            color: "#666"
            opacity: 0.4
            visible: false
    }

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: if(mouse.button & Qt.RightButton) {parent.zoomReset()}
        onPressed: {rectang.x = mouseX; rectang.y = mouseY; rectang.visible = true}
        onMouseXChanged: {rectang.width = mouseX - rectang.x}
        onMouseYChanged: {rectang.height = mouseY - rectang.y}
        onReleased:
        {
            parent.zoomIn(Qt.rect(rectang.x, rectang.y, rectang.width, rectang.height))
            rectang.visible = false
        }
    }
}
