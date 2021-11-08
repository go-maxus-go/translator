#pragma once

#include <QString>


class ITranslationQueryBuilder
{
public:
    virtual ~ITranslationQueryBuilder() = default;

    virtual QString build(
        QString languageFrom,
        QString languageTo,
        QString text) const = 0;
};
