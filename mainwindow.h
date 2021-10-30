#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "graph.h"

#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
    virtual ~MainWindow();
public:
    void initiateTimer();
    void connectToAPI();
    void checkAPIConnection(QNetworkReply*);
    bool isConnected();
private slots:
    void on_TempRadioBtn_clicked();
    void on_HumidRadioBtn_clicked();
    void on_PresRadioBtn_clicked();

private:
    int temp;
    int humid;
    int pres;

    bool connected = false;

    Ui::MainWindow *ui;
    QTimer *dataTimer;
    QTimer *clearDataTimer;
    graph *graphy;

    QUrl url;
    QString myStartupUrl = "http://81.207.176.52:8081/api/mock?limit=2/";
    QString myUrl = "http://81.207.176.52:8081/api/mock?limit=1/";
    QJsonObject rootObj;
    QJsonDocument document;
    QString createdAt;



};
#endif // MAINWINDOW_H

//https://www.youtube.com/watch?v=RjwxKpzYnFE
//https://www.youtube.com/watch?v=y-FXTT5Xn2A
