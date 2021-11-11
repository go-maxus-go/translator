#pragma once

#include <functional>

#include <QString>
#include <QByteArray>


class INetworkManager
{
public:
    virtual ~INetworkManager() = default;

    virtual void sendQuery(
        QString query,
        std::function<void(QByteArray, QString)> responseCallback,
        std::function<void(QString)> errorCallback = [](auto&&){}) = 0;

    virtual int activeQueryCount() const = 0;
};
