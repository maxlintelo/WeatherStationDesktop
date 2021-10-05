#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QTimer>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    void createTemperatureGraph();
    void createHumidityGraph();
    void createPressureGraph();
    void initiateTimer();
public slots:
    void graphUpdateEvent();
    void graphClearEvent();
private slots:
    void on_TemperatureButton_clicked();
    void on_PressureButton_clicked();
    void on_HumidityButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *dataTimer;
    QTimer *clearDataTimer;

    //temperature graph
    QLineSeries *tempSeries;
    QChart *tempChart;
    QValueAxis *tempAxisX;
    QValueAxis *tempAxisY;
    QChartView *tempChartView;

    //humidity graph
    QLineSeries *humidSeries;
    QChart *humidChart;
    QValueAxis *humidAxisX;
    QValueAxis *humidAxisY;
    QChartView *humidChartView;

    QLineSeries *presSeries;
    QChart *presChart;
    QValueAxis *presAxisX;
    QValueAxis *presAxisY;
    QChartView *presChartView;


};
#endif // MAINWINDOW_H

//https://www.youtube.com/watch?v=RjwxKpzYnFE
//https://www.youtube.com/watch?v=y-FXTT5Xn2A
