#ifndef SENSORCHART_H
#define SENSORCHART_H

#include <QChart>
#include <QObject>

#include <QValueAxis>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QChart>

QT_CHARTS_USE_NAMESPACE

enum class SensorType {
    TemperatureChart,
    HumidityChart,
    PressureChart
};

class SensorChart : public QChart
{
    Q_OBJECT
public:
    explicit SensorChart(SensorType);
    virtual ~SensorChart();
public:
    QChart* getChart();
    QLineSeries* getSeries();
public:
    void setTimeMin(QDateTime);
    void setTimeMax(QDateTime);
    void setTimeRange(QDateTime, QDateTime);
    QDateTime* getTimeMin();
    QDateTime* getTimeMax();
    void setValueMin(int);
    void setValueMax(int);
    void setValueRange(int, int);
private:
    QChart* chart;
    QLineSeries* series;
    QValueAxis* yAxis;
    QDateTimeAxis* xAxis;
    QDateTime* beginTime;
    QDateTime* endTime;
};

#endif // SENSORCHART_H
