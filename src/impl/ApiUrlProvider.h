#pragma once

#include "IApiUrlProvider.h"


class ApiUrlProvider : public IApiUrlProvider
{
public:
    ApiUrlProvider()
        : m_apiUrl{"https://api.mymemory.translated.net/get"}
    {}

    QString apiUrl() const override
    {
        return m_apiUrl;
    }

private:
    QString m_apiUrl;
};
