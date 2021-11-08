#pragma once

#include "ILanguageProvider.h"


class LanguageProvider : public ILanguageProvider
{
public:
    LanguageProvider()
        : m_availableLanguages{"en", "de", "ru", "fr"}
    {}

    QStringList availableLanguages() const
    {
        return m_availableLanguages;
    }

private:
    QStringList m_availableLanguages;
};
