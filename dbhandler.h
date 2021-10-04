#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>

class DbHandler
        : public QMainWindow
{
public:
    DbHandler();
    virtual ~DbHandler() = default;
public:
    void fetchAllResults();
private:
    QString dbUrlString = "http://81.207.176.52:8081/api/v1";
private slots:
    QJsonArray downloadFinished(QNetworkReply*);
};

#endif // DBHANDLER_H
