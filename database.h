#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

#include <QUrl>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include <QJsonArray>
#include <QJsonDocument>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    virtual ~DataBase() = default;
public:
    void getValues(int);

signals:
    void valueReceived(QJsonArray);

private:
    QString* urlString;
    QNetworkAccessManager* manager;
};

#endif // DATABASE_H
