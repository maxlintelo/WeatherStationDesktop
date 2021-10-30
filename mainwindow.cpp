#include "mainwindow.h"
#include "ui_mainwindow.h"

//add base-line 0 and -20 line!
int tempMin = -20;
int tempMax = 40;
int humidMin = 0;
int humidMax = 100;
int presMin = 960;
int presMax = 1060;

static int xValue = 5;
static int dataRequestValue = 0;

int graphSize = 610;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createTemperatureGraph();
    createHumidityGraph();
    createPressureGraph();
    connectToAPI();
    initiateTimer();
    ui->TempRadioBtn->setChecked(true);
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

    tempChart = new QChart();
    tempChart->legend()->show();
    tempChart->addSeries(tempSeries);
    tempChart->setTitle("Temperature");

    tempAxisX = new QDateTimeAxis();
    tempAxisX->setFormat("hh:mm:ss");
    tempAxisX->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
    tempChart->addAxis(tempAxisX, Qt::AlignBottom);
    tempSeries->attachAxis(tempAxisX);

    tempAxisY = new QValueAxis();
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

    humidChart = new QChart();
    humidChart->legend()->show();
    humidChart->addSeries(humidSeries);
    humidChart->setTitle("Humidity");

    humidAxisX = new QDateTimeAxis();
    humidAxisX->setFormat("hh:mm:ss");
    humidAxisX->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
    humidChart->addAxis(humidAxisX, Qt::AlignBottom);
    humidSeries->attachAxis(humidAxisX);

    humidAxisY = new QValueAxis();
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

    presChart = new QChart();
    presChart->legend()->show();
    presChart->addSeries(presSeries);
    presChart->setTitle("Pressure");

    presAxisX = new QDateTimeAxis();
    presAxisX->setFormat("hh:mm:ss");
    presAxisX->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
    presChart->addAxis(presAxisX, Qt::AlignBottom);
    presSeries->attachAxis(presAxisX);

    presAxisY = new QValueAxis();
    presAxisY->setRange(presMin,presMax);
    presChart->addAxis(presAxisY, Qt::AlignLeft);
    presSeries->attachAxis(presAxisY);

    presChart->legend()->setVisible(true);
    presChart->legend()->setAlignment(Qt::AlignBottom);

    presChartView = new QChartView(presChart);
    presChartView->setRenderHint(QPainter::Antialiasing);
    presChartView->resize(graphSize, graphSize);
    presChartView->setParent(ui->graphPressure);
    ui->graphPressure->setVisible(FALSE);
}

void MainWindow::initiateTimer(){
    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()),this, SLOT(graphUpdateEvent()));
    dataTimer->start(5000);

    clearDataTimer = new QTimer(this);
    connect(clearDataTimer, SIGNAL(timeout()), this, SLOT(graphClearEvent()));
    clearDataTimer->start(64500);
}

void MainWindow::graphUpdateEvent(){
    connectToAPI();

    if (connected == false){
        qDebug() << "Not connected to the internet.";
    } else {
        tempSeries->append(xValue, temp);
        humidSeries->append(xValue, humid);
        presSeries->append(xValue, pres);
        xValue = xValue + 5;
    }
}

void MainWindow::graphClearEvent(){
    tempSeries->clear();
    humidSeries->clear();
    presSeries->clear();
    xValue = 0;
}

void MainWindow::connectToAPI(){
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::checkAPIConnection);

    if (dataRequestValue == 0){
        url = QUrl(myStartupUrl);
    }else {
        url = QUrl(myUrl);
    }
    QNetworkRequest request(url);
    manager->get(request);
}

void MainWindow::checkAPIConnection(QNetworkReply *reply){
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Statuscode from server: " + QVariant(statusCode).toString();
    if (statusCode == 0){
        connected = false;
    } else {
        connected = true;
        document = QJsonDocument::fromJson(reply->readAll());
        rootObj = document.object();
        foreach(const QJsonValue & v, document.array()) {
            temp = v["temperature"].toString().toFloat();
            humid = v["humidity"].toString().toFloat();
            pres = v["pressure"].toString().toFloat();
            createdAt = v["createdAt"].toString();
            qDebug() << "-- Doc" << dataRequestValue << "--\nTemp:" << temp << "\nHumid:" << humid << "\nPress:" << pres << "\nCreatedAt:" << createdAt << "\n";
            if(dataRequestValue == 0){
                tempSeries->append(0,temp);
                humidSeries->append(0, humid);
                presSeries->append(0, pres);
            }
            dataRequestValue++;
        }
    }
}

void MainWindow::on_TempRadioBtn_clicked()
{
    ui->graphTemperature->setVisible(TRUE);
    ui->graphHumidity->setVisible(FALSE);
    ui->graphPressure->setVisible(FALSE);
}


void MainWindow::on_HumidRadioBtn_clicked()
{
    ui->graphTemperature->setVisible(FALSE);
    ui->graphHumidity->setVisible(TRUE);
    ui->graphPressure->setVisible(FALSE);
}


void MainWindow::on_PresRadioBtn_clicked()
{
    ui->graphPressure->setVisible(TRUE);
    ui->graphHumidity->setVisible(FALSE);
    ui->graphTemperature->setVisible(FALSE);
}

