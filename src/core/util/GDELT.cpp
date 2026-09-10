//
// Created by Dmitri on 2026-09-10.
//

#include "GDELT.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <iostream>


void GDELT::Search(const QString &query, Callback callback) {
    QUrl qurl = QUrl("https://api.gdeltproject.org/api/v2/doc/doc");

    QUrlQuery params;
    params.addQueryItem("query", query);
    params.addQueryItem("mode", "artlist");
    params.addQueryItem("maxrecords", "100");
    params.addQueryItem("timespan", "1y");
    params.addQueryItem("format", "json");

    qurl.setQuery(params);
    QNetworkReply* reply = Manager()->get(QNetworkRequest(qurl));

    QObject::connect(reply, &QNetworkReply::finished, [reply, callback]() {
        QList<MaritimeIncident> incidents;

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            reply->deleteLater();
            callback(incidents);
            return;
        }

        QByteArray data = reply->readAll();
        qDebug() << "HTTP: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "Response: " << data.left(1000);

        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        QJsonArray articles = json.object()["articles"].toArray();

        for (const QJsonValue& val : articles) {

            QJsonObject article = val.toObject();

            MaritimeIncident incident{};
            incident.desc = article["title"].toString();
            incident.source = article["url"].toString();

            incidents.append(incident);
        }

        reply->deleteLater();
        callback(incidents);
    });
}

QNetworkAccessManager* GDELT::Manager() {
    static QNetworkAccessManager manager;
    return &manager;
}
