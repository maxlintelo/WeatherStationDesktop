#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    urlString = new QString("http://81.207.176.52:8081/api/v3?limit=");
    manager = new QNetworkAccessManager(this);
}

void DataBase::getValues(int limit = 0) {
    QString newString = *urlString + QString::number(limit);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(newString)));
    QEventLoop loop;
    //cSIGNAL(readyRead()) or SIGNAL(finished())
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    emit valueReceived(QJsonDocument::fromJson(reply->readAll()).array());
}
