#include "mainwindow.h"
#include "ui_mainwindow.h"

//add base-line 0 and -20 line!
int timeMin = 0;
int timeMax = 60;

int tempMin = -20;
int tempMax = 40;
int humidMin = 0;
int humidMax = 100;
int presMin = 960;
int presMax = 1060;

static int i = 1;

int graphSize = 610;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createTemperatureGraph();
    createHumidityGraph();
    createPressureGraph();
    initiateTimer();
    connectToAPI();

}


MainWindow::~MainWindow()
{
    //main
    delete ui;
    delete dataTimer;
    delete clearDataTimer;

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

    //pressure graph
    delete presSeries;
    delete presChart;
    delete presAxisX;
    delete presAxisY;
    delete presChartView;
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
    ui->graphTemperature->setVisible(TRUE);
}

void MainWindow::createHumidityGraph(){
    humidSeries = new QLineSeries();
    humidSeries->append(0,50);

    humidChart = new QChart();
    humidChart->legend()->show();
    humidChart->addSeries(humidSeries);
    //chart->createDefaultAxes();
    humidChart->setTitle("Humidity");

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
    humidChartView->resize(620, graphSize);
    humidChartView->setParent(ui->graphHumidity);
    ui->graphHumidity->setVisible(FALSE);
}

void MainWindow::createPressureGraph(){
    presSeries = new QLineSeries();
    presSeries->append(0,998);

    presChart = new QChart();
    presChart->legend()->show();
    presChart->addSeries(presSeries);
    presChart->setTitle("Pressure");

    presAxisX = new QValueAxis;
    presAxisX->setRange(timeMin,timeMax);
    presChart->addAxis(presAxisX, Qt::AlignBottom);
    presSeries->attachAxis(presAxisX);

    presAxisY = new QValueAxis;
    presAxisY->setRange(presMin,presMax);
    presChart->addAxis(presAxisY, Qt::AlignLeft);
    presSeries->attachAxis(presAxisY);

    presChart->legend()->setVisible(true);
    presChart->legend()->setAlignment(Qt::AlignBottom);

    presChartView = new QChartView(presChart);
    presChartView->setRenderHint(QPainter::Antialiasing);
    presChartView->resize(620, graphSize);
    presChartView->setParent(ui->graphPressure);
    ui->graphPressure->setVisible(FALSE);
}

void MainWindow::initiateTimer(){
    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()),this, SLOT(graphUpdateEvent()));
    dataTimer->start(1000);

    clearDataTimer = new QTimer(this);
    connect(clearDataTimer, SIGNAL(timeout()), this, SLOT(graphClearEvent()));
    clearDataTimer->start(60500);
}

void MainWindow::graphUpdateEvent(){
     tempSeries->append(i, 20);
     humidSeries->append(i, 80);
     presSeries->append(i, 990);
     i++;
}

void MainWindow::graphClearEvent(){
    tempSeries->clear();
    humidSeries->clear();
    presSeries->clear();
    i = 0;
}

void MainWindow::on_TemperatureButton_clicked()
{
    ui->graphTemperature->setVisible(TRUE);
    ui->graphHumidity->setVisible(FALSE);
    ui->graphPressure->setVisible(FALSE);
}

void MainWindow::on_HumidityButton_clicked()
{
    ui->graphTemperature->setVisible(FALSE);
    ui->graphHumidity->setVisible(TRUE);
    ui->graphPressure->setVisible(FALSE);
}

void MainWindow::on_PressureButton_clicked()
{
    ui->graphPressure->setVisible(TRUE);
    ui->graphHumidity->setVisible(FALSE);
    ui->graphTemperature->setVisible(FALSE);
}

void MainWindow::connectToAPI(){
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::checkAPIConnection);

    const QUrl url = QUrl(myUrl);
    QNetworkRequest request(url);
    manager->get(request);
}

void MainWindow::checkAPIConnection(QNetworkReply *reply){
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Statuscode from server: " + QVariant(statusCode).toString();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject rootObj = document.object();
    int j = 0;
    foreach(const QJsonValue & v, document.array()) {
        float temp = v["temperature"].toString().toFloat();
        float humid = v["humidity"].toString().toFloat();
        float pres = v["pressure"].toString().toFloat();
        QString createdAt = v["createdAt"].toString();
        qDebug() << "-- Doc" << j << "--\nTemp:" << temp << "\nHumid:" << humid << "\nPress:" << pres << "\nCreatedAt:" << createdAt << "\n";
        j++;
    }
}
