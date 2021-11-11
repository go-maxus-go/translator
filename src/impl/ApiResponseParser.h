#pragma once

#include "IApiResponseParser.h"

#include <exception>

#include <QJsonArray>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>


class ApiResponseParser : public IApiResponseParser
{
public:
    ApiResponse parse(QByteArray response) const override
    {
        const auto jsonResponse = QJsonDocument::fromJson(response);
        const auto jsonObject = jsonResponse.object();

        const auto matches = jsonObject["matches"].toArray();
        const auto match = matches[0].toObject();

        ApiResponse apiResponce;
        apiResponce.source = readField(match, "segment");
        apiResponce.translation = readField(match, "translation");
        apiResponce.sourceLanguage = readField(match, "source");
        apiResponce.targetLanguage = readField(match, "target");

        return makePretty(std::move(apiResponce));
    }

private:
    QString readField(QJsonObject object, QString field) const
    {
        if (!object.contains(field))
            throw std::runtime_error("Invalid json");

        return object[field].toString();
    }

    ApiResponse makePretty(ApiResponse apiResponce) const
    {
        apiResponce.source = makePrettyText(apiResponce.source);
        apiResponce.translation = makePrettyText(apiResponce.translation);
        apiResponce.sourceLanguage = makePrettyLanguage(apiResponce.sourceLanguage);
        apiResponce.targetLanguage = makePrettyLanguage(apiResponce.targetLanguage);

        return apiResponce;
    }

    QString makePrettyText(QString text) const
    {
        auto lines = text.split("&#10;");
        for (int i = 0; i < lines.size(); ++i)
            lines[i] = lines[i].trimmed();

        auto prettyText = lines.join("\n");
        return prettyText;
    }

    QString makePrettyLanguage(QString language) const
    {
        language.truncate(2);
        return language;
    }
};
