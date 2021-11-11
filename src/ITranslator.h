#pragma once

#include <QString>

#include "Observervable.h"
#include "ITranslatorObserver.h"


class ITranslator : public Observable<ITranslatorObserver>
{
public:
    virtual ~ITranslator() = default;

    virtual QString sourceLanguage() const = 0;
    virtual void setSourceLanguage(QString) = 0;

    virtual QString targetLanguage() const = 0;
    virtual void setTargetLanguage(QString) = 0;

    virtual void translate(QString text) = 0;
};
