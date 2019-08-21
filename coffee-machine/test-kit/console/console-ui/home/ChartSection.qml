import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.3
import "../commons"

SwipeView
{
    id: swipeView
    property alias lineChart: lineChart
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true

    function addChartPoint(x, y)
    {
        lineChart.append(x, y)
    }

    function clearChart()
    {
        lineChart.clear()
    }

    ChartView
    {
        id: chartView
        antialiasing: true
        legend.visible: false
        backgroundColor: '#f8f8f8'
        ValueAxis
        {
            id: axisX
            min: 0
            max: project.total
            tickCount: 10
            labelFormat: "%.d"
        }
        ValueAxis
        {
            id: axisY
            min: 0
            max: 2000 //TODO: replace with %error value
            tickCount: 10
            labelFormat: "%.d"
        }
        LineSeries
        {
            id: lineChart
            name: "LineSeries"
            axisX: axisX
            axisY: axisY

            Component.onCompleted:
            {
                axisX.applyNiceNumbers()
                axisY.applyNiceNumbers()
            }
        }

        ToolButton//TODO: delete me (just for test addChartPoint function)
        {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 25
            text: 'Add random point'
            font.pixelSize: 10
            onClicked: addChartPoint(Math.ceil(Math.random() * 150), Math.ceil(Math.random() * 140))
        }
        Button
        {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 15
            anchors.topMargin: 10
            height: 30
            Material.elevation: 0
            text: 'View Log'
            font.pixelSize: 11
            font.bold: true
            onClicked: swipeView.currentIndex = 1
            highlighted: true
        }
    }

    Item
    {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Button
        {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 15
            anchors.topMargin: 10
            height: 30
            Material.elevation: 0
            text: 'View Chart'
            font.pixelSize: 11
            font.bold: true
            onClicked: swipeView.currentIndex = 0
            highlighted: true
            z: 10
        }
        Rectangle
        {
            anchors.fill: parent
            anchors.margins: 10
            color: '#333'
            radius: 5
            ListView
            {
                //TODO:
            }
        }
    }
}
