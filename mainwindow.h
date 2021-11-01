#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include <QDate>
#include <QTime>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>

#include <QTimer>

#include "database.h"
#include "sensorchart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioTemperature_clicked();
    void on_radioHumidity_clicked();
    void on_radioPressure_clicked();

    void on_startDateTime_dateTimeChanged(const QDateTime &dateTime);
    void on_endDateTime_dateTimeChanged(const QDateTime &dateTime);

    void dataTimerCallback();
    void graphTimerCallback();

    void on_pushButton_clicked();

private:
    void replyFinished(QNetworkReply*);

private:
    Ui::MainWindow *ui;
    QChartView* chartView;

    DataBase* db;
    void updateSeries(QJsonArray);

    SensorChart* temperatureChart;
    SensorChart* humidityChart;
    SensorChart* pressureChart;

    QTimer* dataTimer;
    QTimer* graphTimer;

    QDateTime* startTime;
    QDateTime* endTime;
    QDateTime* previousTime;
};
#endif // MAINWINDOW_H
