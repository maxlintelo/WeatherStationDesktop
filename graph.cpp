#include "graph.h"
#include "mainwindow.h"

static int xValue = 5;

graph::graph()
{

}

graph::~graph()
{
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



void graph::createTemperatureGraph(QFrame *tempGraph){
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
    tempAxisY->setRange(TEMP_MIN,TEMP_MAX);
    tempChart->addAxis(tempAxisY, Qt::AlignLeft);
    tempSeries->attachAxis(tempAxisY);

    tempChart->legend()->setVisible(true);
    tempChart->legend()->setAlignment(Qt::AlignBottom);

    tempChartView = new QChartView(tempChart);
    tempChartView->setRenderHint(QPainter::Antialiasing);
    tempChartView->resize(GRAPH_SIZE, GRAPH_SIZE);
    tempChartView->setParent(tempGraph);
    tempGraph->setVisible(TRUE);
}

void graph::createHumidityGraph(QFrame *humidGraph){
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
    humidAxisY->setRange(HUMID_MIN,HUMID_MAX);
    humidChart->addAxis(humidAxisY, Qt::AlignLeft);
    humidSeries->attachAxis(humidAxisY);

    humidChart->legend()->setVisible(true);
    humidChart->legend()->setAlignment(Qt::AlignBottom);

    humidChartView = new QChartView(humidChart);
    humidChartView->setRenderHint(QPainter::Antialiasing);
    humidChartView->resize(620, GRAPH_SIZE);
    humidChartView->setParent(humidGraph);
    humidGraph->setVisible(FALSE);
}

void graph::createPressureGraph(QFrame *presGraph){
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
    presAxisY->setRange(PRES_MIN,PRES_MAX);
    presChart->addAxis(presAxisY, Qt::AlignLeft);
    presSeries->attachAxis(presAxisY);

    presChart->legend()->setVisible(true);
    presChart->legend()->setAlignment(Qt::AlignBottom);

    presChartView = new QChartView(presChart);
    presChartView->setRenderHint(QPainter::Antialiasing);
    presChartView->resize(GRAPH_SIZE, GRAPH_SIZE);
    presChartView->setParent(presGraph);
    presGraph->setVisible(FALSE);
}

void graph::graphUpdateEvent(MainWindow *windowy, int temp, int humid, int pres){
    windowy->connectToAPI();

    //if ! changing
    qDebug() << temp;
    qDebug() << humid;
    qDebug() << pres;
    //make references of input values

    if (windowy->isConnected() == false){
        qDebug() << "Not connected to the internet.";
    } else {
        tempSeries->append(xValue, temp);
        humidSeries->append(xValue, humid);
        presSeries->append(xValue, pres);
        xValue = xValue + 5;
    }
}

void graph::graphClearEvent(){
    tempSeries->clear();
    humidSeries->clear();
    presSeries->clear();
    xValue = 0;
}

void graph::appendData(int temp, int humid, int pres)
{
    tempSeries->append(0,temp);
    humidSeries->append(0, humid);
    presSeries->append(0, pres);
}
