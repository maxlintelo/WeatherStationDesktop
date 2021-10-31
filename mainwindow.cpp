#include "mainwindow.h"
#include "ui_mainwindow.h"

static int sUpdateSpeed = 1;
static int dataRequestValue = 0;

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

    tempAxisX = new QValueAxis();
    //tempAxisX->setFormat("hh:mm:ss");
    tempAxisX->setRange(MIN_TIME, MAX_TIME);
    tempAxisX->setTitleText("Time (s)");
    tempChart->addAxis(tempAxisX, Qt::AlignBottom);
    tempSeries->attachAxis(tempAxisX);

    tempAxisY = new QValueAxis();
    tempAxisY->setRange(TEMP_MIN,TEMP_MAX);
    tempAxisY->setTitleText("Temperature (C)");
    tempChart->addAxis(tempAxisY, Qt::AlignLeft);
    tempSeries->attachAxis(tempAxisY);

    tempChart->legend()->setVisible(false);

    tempChartView = new QChartView(tempChart);
    tempChartView->setRenderHint(QPainter::Antialiasing);
    tempChartView->resize(GRAPH_SIZE, GRAPH_SIZE);
    tempChartView->setParent(ui->graphTemperature);
    ui->graphTemperature->setVisible(TRUE);
}

void MainWindow::createHumidityGraph(){
    humidSeries = new QLineSeries();

    humidChart = new QChart();
    humidChart->legend()->show();
    humidChart->addSeries(humidSeries);
    humidChart->setTitle("Humidity");

    humidAxisX = new QValueAxis();
    //humidAxisX->setFormat("hh:mm:ss");
    humidAxisX->setRange(MIN_TIME, MAX_TIME);
    humidAxisX->setTitleText("Time (s)");
    humidChart->addAxis(humidAxisX, Qt::AlignBottom);
    humidSeries->attachAxis(humidAxisX);

    humidAxisY = new QValueAxis();
    humidAxisY->setRange(HUMID_MIN,HUMID_MAX);
    humidAxisY->setTitleText("Humidity (%)");
    humidChart->addAxis(humidAxisY, Qt::AlignLeft);
    humidSeries->attachAxis(humidAxisY);

    humidChart->legend()->setVisible(false);

    humidChartView = new QChartView(humidChart);
    humidChartView->setRenderHint(QPainter::Antialiasing);
    humidChartView->resize(620, GRAPH_SIZE);
    humidChartView->setParent(ui->graphHumidity);
    ui->graphHumidity->setVisible(FALSE);
}

void MainWindow::createPressureGraph(){
    presSeries = new QLineSeries();

    presChart = new QChart();
    presChart->legend()->show();
    presChart->addSeries(presSeries);
    presChart->setTitle("Pressure");

    presAxisX = new QValueAxis();
    //presAxisX->setFormat("hh:mm:ss");
    presAxisX->setRange(MIN_TIME, MAX_TIME);
    presAxisX->setTitleText("Time (s)");
    presChart->addAxis(presAxisX, Qt::AlignBottom);
    presSeries->attachAxis(presAxisX);

    presAxisY = new QValueAxis();
    presAxisY->setRange(PRES_MIN,PRES_MAX);
    presAxisY->setTitleText("Pressure (hPa)");
    presChart->addAxis(presAxisY, Qt::AlignLeft);
    presSeries->attachAxis(presAxisY);

    presChart->legend()->setVisible(false);

    presChartView = new QChartView(presChart);
    presChartView->setRenderHint(QPainter::Antialiasing);
    presChartView->resize(GRAPH_SIZE, GRAPH_SIZE);
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
    connectToAPI();
    if (connected == false){
        qDebug() << "Could not connect to server";
    } else {
        tempSeries->append(sUpdateSpeed, temp);
        humidSeries->append(sUpdateSpeed, humid);
        presSeries->append(sUpdateSpeed, pres);
        sUpdateSpeed = sUpdateSpeed + 1;
    }
}

void MainWindow::graphClearEvent(){
    tempSeries->clear();
    humidSeries->clear();
    presSeries->clear();
    sUpdateSpeed = 0;
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
            temp = v["temperature"].toString().toInt();
            humid = v["humidity"].toString().toInt();
            pres = v["pressure"].toString().toInt();
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

