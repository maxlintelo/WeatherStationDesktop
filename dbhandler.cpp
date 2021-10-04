#include "dbhandler.h"

DbHandler::DbHandler()
{

}

void DbHandler::fetchAllResults() {
    QNetworkAccessManager* man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &DbHandler::downloadFinished);
    const QUrl dbUrl = QUrl(dbUrlString);
    QNetworkRequest request(dbUrl);
    man->get(request);
}

QJsonArray DbHandler::downloadFinished(QNetworkReply* reply) {
    return QJsonDocument::fromJson(reply->readAll()).array();
}
