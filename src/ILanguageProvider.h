#pragma once

#include <QStringList>


class ILanguageProvider
{
public:
    virtual ~ILanguageProvider() = default;

    virtual QStringList availableLanguages() const = 0;
};
