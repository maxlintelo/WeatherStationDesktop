#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

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
    void connectToAPI();
    void checkAPIConnection(/*QNetworkReply* */);
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
    QString myUrl = "https://www.google.nl/url?sa=i&url=https%3A%2F%2Fwww.amazon.in%2FAmazonBasics-Everyday-Alkaline-Batteries-4-Pack%2Fdp%2FB00LH3DREU&psig=AOvVaw06uKSu2JnAjWekte3swfWn&ust=1633543494292000&source=images&cd=vfe&ved=0CAkQjRxqFwoTCKj_3fbts_MCFQAAAAAdAAAAABAD";

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
