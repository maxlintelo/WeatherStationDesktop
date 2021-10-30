#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>
#include <QtCharts>
#include <QDateTimeAxis>
#include <QChartView>
#include <QLineSeries>

#define TEMP_MIN -20
#define TEMP_MAX 40
#define HUMID_MIN 0
#define HUMID_MAX 100
#define PRES_MIN 960
#define PRES_MAX 1060
#define GRAPH_SIZE 610

//forward decleration of a class
class MainWindow;

class graph
{
public:
    graph();
    virtual ~graph();
public:
    void createTemperatureGraph(QFrame *tempGraph);
    void createHumidityGraph(QFrame *humidGraph);
    void createPressureGraph(QFrame *presGraph);
    void appendData(int, int, int);
public slots:
    void graphUpdateEvent(MainWindow *windowy, int, int , int);
    void graphClearEvent();
private:
    //temperature graph
    QLineSeries *tempSeries;
    QChart *tempChart;
    QDateTimeAxis *tempAxisX;
    QValueAxis *tempAxisY;
    QChartView *tempChartView;

    //humidity graph
    QLineSeries *humidSeries;
    QChart *humidChart;
    QDateTimeAxis *humidAxisX;
    QValueAxis *humidAxisY;
    QChartView *humidChartView;

    //pressure graph
    QLineSeries *presSeries;
    QChart *presChart;
    QDateTimeAxis *presAxisX;
    QValueAxis *presAxisY;
    QChartView *presChartView;
};

#endif // GRAPH_H
