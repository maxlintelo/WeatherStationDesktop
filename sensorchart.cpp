#include "sensorchart.h"

QT_CHARTS_USE_NAMESPACE

SensorChart::SensorChart(SensorType type)
{
    series = new QLineSeries();
    series->setUseOpenGL(true);

    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    switch (type) {
    case SensorType::TemperatureChart:
        chart->setTitle("Temperature");
        break;
    case SensorType::HumidityChart:
        chart->setTitle("Humidity");
        break;
    case SensorType::PressureChart:
        chart->setTitle("Pressure");
        break;
    default:
        chart->setTitle("Unknown Chart Type");
        break;
    }
    xAxis = new QDateTimeAxis;
    xAxis->setTickCount(6);
    xAxis->setFormat("yyyy-MM-dd HH:mm:ss");
    xAxis->setTitleText("Date");
    chart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    yAxis = new QValueAxis;
    switch (type) {
    case SensorType::TemperatureChart:
        yAxis->setTitleText("Temperature (C)");
        break;
    case SensorType::HumidityChart:
        yAxis->setTitleText("Humidity (%)");
        break;
    case SensorType::PressureChart:
        yAxis->setTitleText("Pressure (hPa)");
        break;
    default:
        yAxis->setTitleText("Unknown Chart Type");
        break;
    }
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);
}

SensorChart::~SensorChart() {
    delete chart;
    delete series;
    delete yAxis;
    delete xAxis;
}

QChart* SensorChart::getChart() {
    return chart;
}
QLineSeries* SensorChart::getSeries() {
    return series;
}

void SensorChart::setTimeMin(QDateTime beginTime) {
    xAxis->setMin(beginTime);
    this->beginTime = &beginTime;
}
QDateTime* SensorChart::getTimeMin() {
    return beginTime;
}
void SensorChart::setTimeMax(QDateTime endTime) {
    xAxis->setMax(endTime);
    this->endTime = &endTime;
}
QDateTime* SensorChart::getTimeMax() {
    return endTime;
}
void SensorChart::setTimeRange(QDateTime beginTime, QDateTime endTime) {
    xAxis->setMin(beginTime);
    this->beginTime = &beginTime;
    xAxis->setMax(endTime);
    this->endTime = &endTime;
}

void SensorChart::setValueMin(int minimum) {
    yAxis->setMin(minimum);
}
void SensorChart::setValueMax(int maximum) {
    yAxis->setMax(maximum);
}
void SensorChart::setValueRange(int minimum, int maximum) {
    yAxis->setMin(minimum);
    yAxis->setMax(maximum);
}
