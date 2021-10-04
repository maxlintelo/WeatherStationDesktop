#include "mainwindow.h"
#include "ui_mainwindow.h"

int tempMin = 0;
int tempMax = 40;
int timeMin = 0;
int timeMax = 60;

int graphSize = 610;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    series = new QLineSeries();

    series->append(0 , 6);
    series->append(2 , 4);
    series->append(3 , 8);
    series->append(7 , 4);
    series->append(10, 5);
    series->append(11, 1);
    series->append(13, 3);
    series->append(17, 6);
    series->append(18, 3);
    series->append(20, 2);

    //*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    chart = new QChart();
    chart->legend()->show();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->setTitle("Temperature");

    axisX = new QValueAxis;
    axisX->setRange(timeMin,timeMax);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setRange(tempMin,tempMax);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(graphSize,graphSize);
    chartView->setParent(ui->horizontalFrame);
}


MainWindow::~MainWindow()
{
    delete ui;

    delete series;
    delete chart;
    delete axisX;
    delete axisY;
    delete chartView;
}


//https://www.youtube.com/watch?v=eS61kziGo1I


void MainWindow::on_pushButton_clicked()
{
    static int i = 22;
    series->append(i, 12);
    i++;
}


void MainWindow::on_pushButton_3_clicked()
{

}
