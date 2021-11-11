#pragma once

#include <QString>

#include "Observervable.h"
#include "ITranslatorObserver.h"


class ITranslator : public Observable<ITranslatorObserver>
{
public:
    virtual ~ITranslator() = default;

    virtual QString languageFrom() const = 0;
    virtual void setLanguageFrom(QString) = 0;

    virtual QString languageTo() const = 0;
    virtual void setLanguageTo(QString) = 0;

    virtual void translate(QString text) = 0;
};
