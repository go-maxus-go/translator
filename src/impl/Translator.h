#pragma once

#include "Tags.h"

#include "ITranslator.h"
#include "ILanguageProvider.h"


class Translator : public ITranslator
{
public:
    using di_deps = std::tuple<LanguageProviderTag>;
    Translator(LanguageProviderPtr languageProvider)
        : m_languageProvider{std::move(languageProvider)}
    {
        const auto languages = m_languageProvider->availableLanguages();
        m_languageFrom = languages[0];
        m_languageTo = languages[1];
    }
public:
    QString languageFrom() const override
    {
        return m_languageFrom;
    }
    void setLanguageFrom(QString language) override
    {
        if (language == m_languageTo)
            std::swap(m_languageFrom, m_languageTo);
        else
            m_languageFrom = language;

        notifyLanguageChanged();
    }

    QString languageTo() const override
    {
        return m_languageTo;
    }
    void setLanguageTo(QString language) override
    {
        if (language == m_languageFrom)
            std::swap(m_languageFrom, m_languageTo);
        else
            m_languageTo = language;

        notifyLanguageChanged();
    }

    void translate(QString text) override
    {

    }

private:
    void notifyLanguageChanged()
    {
        notify([](auto&& obs) { obs.onLanguageChanged(); });
    }

public:
    LanguageProviderPtr m_languageProvider;
    QString m_languageFrom;
    QString m_languageTo;
};
