#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QTimer>
#include <QCoreApplication>

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
    void createGraph();
public slots:
    void graphTimerEvent();
private slots:
    void on_TemperatureButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QLineSeries *series;
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;
    QTimer *dataTimer;
};
#endif // MAINWINDOW_H
