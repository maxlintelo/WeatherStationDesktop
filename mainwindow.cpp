#include "mainwindow.h"
#include "ui_mainwindow.h"

//add base-line 0 and -20 line!
int timeMin = 0;
int timeMax = 60;

int tempMin = 0;
int tempMax = 40;
int humidMin = 0;
int humidMax = 100;

static int i = 1;

int graphSize = 610;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createTemperatureGraph();
    createHumidityGraph();

    Sleep(1000);

    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()),this, SLOT(graphUpdateEvent()));
    dataTimer->start(1000);
}


MainWindow::~MainWindow()
{
    //main
    delete ui;
    delete dataTimer;

    //temperature graph
    delete tempSeries;
    delete tempChart;
    delete tempAxisX;
    delete tempAxisY;
    delete tempChartView;

    //humidity graph
    delete humidSeries;
    delete humidChart;
    delete humidAxisX;
    delete humidAxisY;
    delete humidChartView;
}

//https://www.youtube.com/watch?v=eS61kziGo1I

void MainWindow::createTemperatureGraph(){
    tempSeries = new QLineSeries();
    tempSeries->append(0,16);

    tempChart = new QChart();
    tempChart->legend()->show();
    tempChart->addSeries(tempSeries);
    //chart->createDefaultAxes();
    tempChart->setTitle("Temperature");

    tempAxisX = new QValueAxis;
    tempAxisX->setRange(timeMin,timeMax);
    tempChart->addAxis(tempAxisX, Qt::AlignBottom);
    tempSeries->attachAxis(tempAxisX);

    tempAxisY = new QValueAxis;
    tempAxisY->setRange(tempMin,tempMax);
    tempChart->addAxis(tempAxisY, Qt::AlignLeft);
    tempSeries->attachAxis(tempAxisY);

    tempChart->legend()->setVisible(true);
    tempChart->legend()->setAlignment(Qt::AlignBottom);

    tempChartView = new QChartView(tempChart);
    tempChartView->setRenderHint(QPainter::Antialiasing);
    tempChartView->resize(graphSize, graphSize);
    tempChartView->setParent(ui->graphTemperature);
}

void MainWindow::createHumidityGraph(){
    tempSeries = new QLineSeries();
    humidSeries->append(0,16);

    humidChart = new QChart();
    humidChart->legend()->show();
    humidChart->addSeries(humidSeries);
    //chart->createDefaultAxes();
    humidChart->setTitle("humiderature");

    humidAxisX = new QValueAxis;
    humidAxisX->setRange(timeMin,timeMax);
    humidChart->addAxis(humidAxisX, Qt::AlignBottom);
    humidSeries->attachAxis(humidAxisX);

    humidAxisY = new QValueAxis;
    humidAxisY->setRange(humidMin,humidMax);
    humidChart->addAxis(humidAxisY, Qt::AlignLeft);
    humidSeries->attachAxis(humidAxisY);

    humidChart->legend()->setVisible(true);
    humidChart->legend()->setAlignment(Qt::AlignBottom);

    humidChartView = new QChartView(humidChart);
    humidChartView->setRenderHint(QPainter::Antialiasing);
    humidChartView->resize(graphSize, graphSize);
    humidChartView->setParent(ui->graphHumidity);
}

void MainWindow::graphUpdateEvent(){
     tempSeries->append(i, 20);
     humidSeries->append(i, 59);
     i++;
}

void MainWindow::on_TemperatureButton_clicked()
{
    ui->graphTemperature->setVisible(TRUE);
    ui->graphHumidity->setVisible(FALSE);
    tempSeries->show();
}

void MainWindow::on_HumidityButton_clicked()
{
    ui->graphHumidity->setVisible(TRUE);
    tempSeries->hide();
}

void MainWindow::on_pushButton_3_clicked()
{

}



