#pragma once

#include <QString>


class ITranslationQueryBuilder
{
public:
    virtual ~ITranslationQueryBuilder() = default;

    virtual QString build(
        QString sourceLanguage,
        QString targetLanguage,
        QString text) const = 0;
};
