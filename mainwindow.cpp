#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph.h"

//add base-line 0 and -20 line!



static int dataRequestValue = 0;



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphy = new graph();
    graphy->createTemperatureGraph(ui->graphTemperature);
    graphy->createHumidityGraph(ui->graphHumidity);
    graphy->createPressureGraph(ui->graphPressure);
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
    delete graphy;
}

//https://www.youtube.com/watch?v=eS61kziGo1I


void MainWindow::initiateTimer(){
    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()),this, SLOT(graph::graphUpdateEvent(this, temp, humid, pres)));
    dataTimer->start(5000);

    clearDataTimer = new QTimer(this);
    connect(clearDataTimer, SIGNAL(timeout()), this, SLOT(graph::graphClearEvent()));
    clearDataTimer->start(64500);
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
            temp = v["temperature"].toInt();
            humid = v["humidity"].toInt();
            pres = v["pressure"].toInt();
            createdAt = v["createdAt"].toString();
            qDebug() << "-- Doc" << dataRequestValue << "--\nTemp:" << temp << "\nHumid:" << humid << "\nPress:" << pres << "\nCreatedAt:" << createdAt << "\n";
            if(dataRequestValue == 0){
                graphy->appendData(temp, humid, pres);
            }
            dataRequestValue++;
        }
    }
}

bool MainWindow::isConnected()
{
    return connected;
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

