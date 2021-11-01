#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    // Generate new access manager
    manager = new QNetworkAccessManager(this);
    urlString = new QString("http://81.207.176.52:8081/api/v3?limit=");
}

void DataBase::getValues(int limit = 0) {
    // Put the limit in the string ?limit=limit
    QString newString = *urlString + QString::number(limit);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(newString)));
    // Eventloop for async data handling
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    // Emit our signal when ready
    emit valueReceived(QJsonDocument::fromJson(reply->readAll()).array());
}
