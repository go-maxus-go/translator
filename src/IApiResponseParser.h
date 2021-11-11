#pragma once

#include <QByteArray>

#include "ApiResponse.h"


class IApiResponseParser
{
public:
    virtual ~IApiResponseParser() = default;

    virtual ApiResponse parse(QByteArray response) const = 0;
};
