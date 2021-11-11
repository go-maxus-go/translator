#pragma once

#include "Tags.h"

#include "ITranslator.h"
#include "INetworkManager.h"
#include "ILanguageProvider.h"
#include "IApiResponseParser.h"
#include "ITranslationQueryBuilder.h"


class Translator : public ITranslator
{
public:
    using di_deps = std::tuple<
        NetworkManagerTag,
        LanguageProviderTag,
        ApiResponseParserTag,
        TranslationQueryBuilderTag>;
    Translator(
        NetworkManagerPtr networkManager,
        LanguageProviderPtr languageProvider,
        ApiResponseParserPtr apiResponseParser,
        TranslationQueryBuilderPtr translationQueryBuilder)
        : m_networkManager{std::move(networkManager)}
        , m_languageProvider{std::move(languageProvider)}
        , m_apiResponseParser{std::move(apiResponseParser)}
        , m_translationQueryBuilder{std::move(translationQueryBuilder)}
    {
        const auto languages = m_languageProvider->availableLanguages();
        m_sourceLanguage = languages[0];
        m_targetLanguage = languages[1];
    }

public:
    QString sourceLanguage() const override
    {
        return m_sourceLanguage;
    }

    void setSourceLanguage(QString language) override
    {
        if (language == m_sourceLanguage)
            return;

        if (language == m_targetLanguage)
            std::swap(m_sourceLanguage, m_targetLanguage);
        else
            m_sourceLanguage = language;

        notifyLanguageChanged();
    }

    QString targetLanguage() const override
    {
        return m_targetLanguage;
    }

    void setTargetLanguage(QString language) override
    {
        if (language == m_targetLanguage)
            return;

        if (language == m_sourceLanguage)
            std::swap(m_sourceLanguage, m_targetLanguage);
        else
            m_targetLanguage = language;

        notifyLanguageChanged();
    }

    void translate(QString text) override
    {
        const auto query = m_translationQueryBuilder->build(
            sourceLanguage(),
            targetLanguage(),
            text);
        m_networkManager->sendQuery(query, [this](auto response, auto){
            const auto apiResponse = m_apiResponseParser->parse(response);
            notifyTranslated(apiResponse );
        });
    }

private:
    void notifyLanguageChanged()
    {
        notify([](auto&& obs) { obs.onLanguageChanged(); });
    }

    void notifyTranslated(ApiResponse apiResponse)
    {
        notify([&apiResponse](auto&& obs) { obs.onTranslated(apiResponse); });
    }

public:
    NetworkManagerPtr m_networkManager;
    LanguageProviderPtr m_languageProvider;
    ApiResponseParserPtr m_apiResponseParser;
    TranslationQueryBuilderPtr m_translationQueryBuilder;

    QString m_sourceLanguage;
    QString m_targetLanguage;
};
