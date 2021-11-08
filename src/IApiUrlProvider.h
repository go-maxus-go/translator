#pragma once

#include <QString>


class IApiUrlProvider
{
public:
    virtual ~IApiUrlProvider() = default;

    virtual QString apiUrl() const = 0;
};
