#pragma once

#include <functional>

#include <QString>


class INetworkManager
{
public:
    virtual ~INetworkManager() = default;

    virtual void sendQuery(
        QString query,
        std::function<void(QString, QString)> responseCallback,
        std::function<void(QString)> errorCallback = [](auto&&){}) = 0;

    virtual int activeQueryCount() const = 0;
};
