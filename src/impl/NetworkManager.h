#pragma once

#include "INetworkManager.h"

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>


class NetworkManager : public INetworkManager, QObject
{
public:
    void sendQuery(
        QString query,
        std::function<void(QByteArray, QString)> responseCallback,
        std::function<void(QString)> errorCallback = [](auto&&){}) override
    {
        ++m_activeQueryCount;

        QNetworkRequest request(query);
        auto reply = m_networkAccessManager.get(request);
        connect(reply, &QNetworkReply::finished, this,
            [this, reply, query, responseCallback, errorCallback]() {
                const auto error = reply->error();
                if (error == QNetworkReply::NetworkError::NoError) {
                    auto data = reply->readAll();
                    responseCallback(data, query);
                }
                else
                    errorCallback(query);

                reply->deleteLater();
                --m_activeQueryCount;
            }
        );
    }

    int activeQueryCount() const override
    {
        return m_activeQueryCount;
    }

private:
    QNetworkAccessManager m_networkAccessManager;
    int m_activeQueryCount = 0;
};
