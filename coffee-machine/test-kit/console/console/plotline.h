#ifndef PLOTLINE_H
#define PLOTLINE_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QXYSeries>
#include <QDebug>
using namespace QtCharts;

struct IntPoint
{
    IntPoint(uint32_t x, double y)
    {
        this->x = x;
        this->y = y;
    }
    uint32_t x;
    double y;
};

class DataLine
{
public:
    DataLine(QXYSeries* series): series_(series){}
    QXYSeries* series(){return series_;}

    virtual void addValue(uint32_t x, int32_t y)=0;
    virtual void flush(uint32_t firstIndex) =0;
protected:
    QXYSeries* series_;
};

class PlotLine: public DataLine
{
public:
    PlotLine(QXYSeries* series): DataLine(series)
    {
        plotData_.resize(1000);
        for (int i=0;i<1000;i++)
        {
            plotData_[i].setX(i);
            plotData_[i].setY(0);
        }
    }
    void addValue(uint32_t x, int32_t y)
    {
        data_<< IntPoint(x,y);
    }
    void flush(uint32_t firstIndex)
    {
        while (data_.first().x < firstIndex)
        {
            data_.removeFirst();
        }
        //QVector<QPointF> tmp(data_.size());
        int i=0;
        for (auto& p: data_)
        {
            plotData_[i].setX(p.x-firstIndex);
            plotData_[i].setY(p.y);
            i++;
            if (i>999) break;
        }
        series_->replace(plotData_);
    }
private:
    QVector<QPointF> plotData_;
    QList<IntPoint> data_;
};

#endif // PLOTLINE_H
