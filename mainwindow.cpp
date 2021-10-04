#include "mainwindow.h"
#include "ui_mainwindow.h"

//add base-line 0 and -20 line!
int tempMin = 0;
int tempMax = 40;
int timeMin = 0;
int timeMax = 60;

static int i = 1;

int graphSize = 610;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createGraph();

    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()),this, SLOT(graphTimerEvent()));
    dataTimer->start(1000);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete series;
    delete chart;
    delete axisX;
    delete axisY;
    delete chartView;
    delete dataTimer;
}

//https://www.youtube.com/watch?v=eS61kziGo1I

void MainWindow::createGraph(){
    series = new QLineSeries();
    series->append(0,16);

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
    chartView->resize(graphSize, graphSize);
    chartView->setParent(ui->horizontalFrame);
}

void MainWindow::graphTimerEvent(){
     series->append(i, 20);
     i++;
}

void MainWindow::on_TemperatureButton_clicked()
{

}


void MainWindow::on_pushButton_3_clicked()
{

}
