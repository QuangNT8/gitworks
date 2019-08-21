import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.2

ApplicationWindow
{
    id: mainWindow
    visible: true
    width: 1500
    height: 1000
	title: qsTr("Firmware Debugger")
	color: '#eee'
	property var commandModel: Console.commands

    function showMessage(text)
    {
        txtConsole.append(text)
    }

	function displayPlot(index, v)
	{
		plotList.itemAt(index).visible = v
	}

	function addSeries(index, name)
	{
		var chart =  plotList.itemAt(index)
        var ret = chart.createSeries(ChartView.SeriesTypeLine, name,chart.xAxis,chart.yAxis);
        ret.useOpenGL = true;
		return ret;
	}

    function setYAxisRange(index, min, max)
    {
        var chart =  plotList.itemAt(index);
        chart.setYRange(min, max);
    }

    function setXAxisRange(index, min, max)
    {
        var chart =  plotList.itemAt(index);
        chart.setXRange(min, max);
    }

    function initSeries()
    {
        plotList.itemAt(0).removeAllSeries();
        plotList.itemAt(1).removeAllSeries();
    }

    Welcome
	{
		id: welcome
		anchors.fill: parent

	}

    RowLayout
    {
		visible:!welcome.visible
        anchors.fill: parent
        spacing: 0

        Rectangle
        {
			color: '#eee'
            Layout.fillWidth: true;
            Layout.fillHeight: true;
			ColumnLayout
            {
				spacing: 0
                anchors.fill: parent
				Repeater
				{
					id: plotList
					model: 2
					PlotView
					{
						Layout.fillHeight: true
						Layout.fillWidth: true
					}
				}
			}
        }

		ColumnLayout
		{
			Layout.maximumWidth: 350
			Layout.fillHeight: true
			spacing: 0

			Serial
			{
				Layout.fillWidth: true
                height: 120
			}

			Rectangle
			{
				Layout.fillWidth: true
				height: 1
				color: '#666'
			}

			Rectangle
			{
				Layout.fillHeight: true
				Layout.fillWidth: true
				color: '#444'
				Terminal
				{
					anchors.fill: parent
                    anchors.margins: 5
					id: txtConsole;
				}
			}
//            Rectangle
//            {
//                Layout.fillWidth: true
//                height: plotAnalyse.height
//                color: '#bbb'
//                PlotAnalyse
//                {
//                    id: plotAnalyse
//                    Layout.fillWidth: true
//                }
//            }
		}
		ControlPanel
		{
			Layout.fillHeight: true
			Layout.maximumWidth: 200
			Layout.minimumWidth: 200
		}

	}
}
