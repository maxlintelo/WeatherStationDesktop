#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>

QT_CHARTS_USE_NAMESPACE

#define INPUT_FREQUENCY 59000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(true);
    db = new DataBase(this);
    connect(db, &DataBase::valueReceived, this, &MainWindow::updateSeries);

    temperatureChart = new SensorChart(SensorType::TemperatureChart);
    humidityChart = new SensorChart(SensorType::HumidityChart);
    pressureChart = new SensorChart(SensorType::PressureChart);

    previousTime = new QDateTime();
    previousTime->setDate(QDateTime::currentDateTime().date());
    previousTime->setTime(QDateTime::currentDateTime().time());
    endTime = new QDateTime();
    endTime->setDate(QDateTime::currentDateTime().date());
    endTime->setTime(QDateTime::currentDateTime().time());
    startTime = new QDateTime();
    startTime->setDate(endTime->date());
    startTime->setTime(endTime->time().addSecs(-900));

    temperatureChart->setValueRange(-20, 50);
    humidityChart->setValueRange(0, 100);
    pressureChart->setValueRange(900, 1100);

    chartView = new QChartView();
    chartView->setChart(temperatureChart->getChart());
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->graphFrame->width(), ui->graphFrame->height());
    chartView->setParent(ui->graphFrame);

    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(dataTimerCallback()));
    dataTimerCallback();
    dataTimer->start(1500);

    graphTimer = new QTimer(this);
    connect(graphTimer, SIGNAL(timeout()), this, SLOT(graphTimerCallback()));
    graphTimerCallback();
    graphTimer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
    delete chartView;

    delete temperatureChart;
    delete humidityChart;
    delete pressureChart;

    delete dataTimer;
    delete graphTimer;

    delete startTime;
    delete endTime;
    delete previousTime;
}

void MainWindow::dataTimerCallback() {
    // This gets called every second by QTimer timer
    qint64 dt = QDateTime::currentMSecsSinceEpoch() - startTime->toMSecsSinceEpoch();
    int lim = dt / INPUT_FREQUENCY;
    db->getValues(lim + 5);
}

void MainWindow::graphTimerCallback() {
    static QDateTime last;
    if (ui->checkBox->isChecked()) {
        qint64 dt;
        if (!last.isNull()) {
            dt = QDateTime::currentMSecsSinceEpoch() - last.toMSecsSinceEpoch();
        } else {
            dt = 0;
        }
        last = QDateTime::currentDateTime();

        // qDebug() << "Dt" << dt << "Current" << QDateTime::currentMSecsSinceEpoch() << "Last" << last.toMSecsSinceEpoch();
        startTime->setTime(startTime->time().addMSecs(dt));
        endTime->setTime(endTime->time().addMSecs(dt));
    }

    ui->startDateTime->setDateTime(*startTime);
    ui->endDateTime->setDateTime(*endTime);
    temperatureChart->setTimeRange(*startTime, *endTime);
    humidityChart->setTimeRange(*startTime, *endTime);
    pressureChart->setTimeRange(*startTime, *endTime);
}

void MainWindow::on_radioTemperature_clicked()
{
    temperatureChart->getSeries()->show();
    humidityChart->getSeries()->hide();
    pressureChart->getSeries()->hide();
    chartView->setChart(temperatureChart->getChart());
}
void MainWindow::on_radioHumidity_clicked()
{
    temperatureChart->getSeries()->hide();
    humidityChart->getSeries()->show();
    pressureChart->getSeries()->hide();
    chartView->setChart(humidityChart->getChart());
}
void MainWindow::on_radioPressure_clicked()
{
    temperatureChart->getSeries()->hide();
    humidityChart->getSeries()->hide();
    pressureChart->getSeries()->show();
    chartView->setChart(pressureChart->getChart());
}

void MainWindow::updateSeries(QJsonArray arr) {
    QList<QPointF> temperaturePoints;
    QList<QPointF> humidityPoints;
    QList<QPointF> pressurePoints;

    foreach(const QJsonValue & v, arr) {
        int temp = v["temperature"].toInt();
        int humid = v["humidity"].toInt();
        int pres = v["pressure"].toInt();
        QDateTime momentInTime = QDateTime::fromString(v["createdAt"].toString(), Qt::ISODate);
        temperaturePoints.append(QPointF(momentInTime.toMSecsSinceEpoch(), temp));
        humidityPoints.append(QPointF(momentInTime.toMSecsSinceEpoch(), humid));
        pressurePoints.append(QPointF(momentInTime.toMSecsSinceEpoch(), pres));
        // qDebug() << "Time:" << momentInTime.toMSecsSinceEpoch() << "-" << temp << humid << pres;
    }

    temperatureChart->getSeries()->replace(temperaturePoints);
    QVector<int> tempVec;
    for (int i = 0; i < temperaturePoints.size(); i++) {
        tempVec.append(temperaturePoints[i].y());
    }
    humidityChart->getSeries()->replace(humidityPoints);
    QVector<int> humidVec;
    for (int i = 0; i < humidityPoints.size(); i++) {
        humidVec.append(humidityPoints[i].y());
    }
    pressureChart->getSeries()->replace(pressurePoints);
    QVector<int> pressureVec;
    for (int i = 0; i < pressurePoints.size(); i++) {
        pressureVec.append(pressurePoints[i].y());
    }

    int tempMin = *std::min_element(tempVec.constBegin(), tempVec.constEnd());
    int tempMax = *std::max_element(tempVec.constBegin(), tempVec.constEnd());
    int humidMin = *std::min_element(humidVec.constBegin(), humidVec.constEnd());
    int humidMax = *std::max_element(humidVec.constBegin(), humidVec.constEnd());
    int pressureMin = *std::min_element(pressureVec.constBegin(), pressureVec.constEnd());
    int pressureMax = *std::max_element(pressureVec.constBegin(), pressureVec.constEnd());

    temperatureChart->setValueRange(tempMin - 2, tempMax + 2);
    humidityChart->setValueRange(humidMin - 4, humidMax + 4);
    pressureChart->setValueRange(pressureMin - 10, pressureMax + 10);
}

void MainWindow::on_startDateTime_dateTimeChanged(const QDateTime &dateTime)
{
    startTime->setDate(dateTime.date());
    startTime->setTime(dateTime.time());
    temperatureChart->setTimeMin(dateTime);
    humidityChart->setTimeMin(dateTime);
    pressureChart->setTimeMin(dateTime);
}

void MainWindow::on_endDateTime_dateTimeChanged(const QDateTime &dateTime)
{
    endTime->setDate(dateTime.date());
    endTime->setTime(dateTime.time());
    temperatureChart->setTimeMax(dateTime);
    humidityChart->setTimeMax(dateTime);
    pressureChart->setTimeMax(dateTime);
}


void MainWindow::on_pushButton_clicked()
{
    ui->checkBox->setChecked(true);
    qint64 dt = QDateTime::currentMSecsSinceEpoch() - endTime->toMSecsSinceEpoch();
    *startTime = startTime->addMSecs(dt);
    *endTime = endTime->addMSecs(dt);
}

