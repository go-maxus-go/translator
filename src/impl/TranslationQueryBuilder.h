#pragma once

#include <QUrl>
#include <QUrlQuery>

#include "Tags.h"
#include "IApiUrlProvider.h"
#include "ITranslationQueryBuilder.h"


class TranslationQueryBuilder : public ITranslationQueryBuilder
{
public:
    using di_deps = ApiUrlProviderTag;
    TranslationQueryBuilder(ApiUrlProviderPtr apiUrlProvider)
        : m_apiUrlProvider{std::move(apiUrlProvider)}
    {}

    QString build(
        QString sourceLanguage,
        QString targetLanguage,
        QString text) const override
    {
        QUrlQuery query;
        query.addQueryItem("q", text);
        auto languagePair = QString("%1|%2").arg(sourceLanguage, targetLanguage);
        query.addQueryItem("langpair", languagePair);

        const auto apiUrl = m_apiUrlProvider->apiUrl();
        QUrl url(apiUrl);
        url.setQuery(query.query());

        return url.toString();
    }

private:
    ApiUrlProviderPtr m_apiUrlProvider;
};
