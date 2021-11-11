#pragma once

#include "ApiResponse.h"


class ITranslatorObserver
{
public:
    virtual ~ITranslatorObserver() = default;

    virtual void onLanguageChanged() {}
    virtual void onTranslated(ApiResponse) {}
};
