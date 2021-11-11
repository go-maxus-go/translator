#include "Translator.h"

#include <catch.hpp>
#include <fakeit.hpp>

#include <QCoreApplication>

#include "Tags.h"
#include "Module.h"

#include "MockUtil.h"

#include "INetworkManager.h"


using namespace fakeit;

TEST_CASE("Integration test. Translator translates text using network")
{
    int argc = 0;
    char* argv[0];
    QCoreApplication app(argc, argv);

    Mock<ITranslatorObserver> observer;
    const auto observerHolder = mock2ptr(observer);
    When(Method(observer, onTranslated)).Do([](auto apiResponse){
        REQUIRE(apiResponse.translation == "Hallo");
    });

    auto context = createContext();
    auto translator = context.resolve<TranslatorTag>();
    translator->setSourceLanguage("en");
    translator->setTargetLanguage("de");
    translator->attach(observerHolder);

    translator->translate("Hello");
    while (context.resolve<NetworkManagerTag>()->activeQueryCount() != 0)
        app.processEvents();

    Verify(Method(observer, onTranslated)).Once();
}

TEST_CASE("Integration test. Translator translates text with mocked network")
{
    Mock<ITranslatorObserver> observer;
    const auto observerHolder = mock2ptr(observer);
    When(Method(observer, onTranslated)).Do([](auto apiResponse){
        REQUIRE(apiResponse.translation == "Hallo");
    });

    Mock<INetworkManager> networkManager;
    When(Method(networkManager, sendQuery)).Do([](auto, auto&& callback, auto){
        callback(R"(
            {
              "matches": [
                {
                  "segment": "Hello",
                  "translation": "Hallo",
                  "source": "en",
                  "target": "de"
                }
              ]
            }
        )", {});
    });

    auto context = createContext();
    context.put<NetworkManagerTag>([&networkManager](auto&&) {
        return mock2ptr(networkManager);
    });

    auto translator = context.resolve<TranslatorTag>();
    translator->attach(observerHolder);

    translator->translate("Hello");

    Verify(Method(observer, onTranslated)).Once();
}

TEST_CASE("Unit test. Translator full")
{
    Mock<INetworkManager> networkManager;
    When(Method(networkManager, sendQuery)).
        Do([](auto, auto callback, auto){ callback({},{}); });

    Mock<IApiResponseParser> apiResponseParser;
    ApiResponse apiResponse;
    apiResponse.translation = "Hallo";
    When(Method(apiResponseParser, parse)).Return(apiResponse);

    Mock<ITranslationQueryBuilder> translationQueryBuilder;
    Fake(Method(translationQueryBuilder, build));

    const auto availableLanguages = QStringList{"de", "en", "ru"};
    Mock<ILanguageProvider> languageProvider;
    When(Method(languageProvider, availableLanguages)).
        Return(availableLanguages);

    auto translator = Translator(
        mock2ptr(networkManager),
        mock2ptr(languageProvider),
        mock2ptr(apiResponseParser),
        mock2ptr(translationQueryBuilder));

    Mock<ITranslatorObserver> observer;
    const auto observerHolder = mock2ptr(observer);
    Fake(Method(observer, onLanguageChanged));

    translator.attach(observerHolder);

    SECTION("When translate method called, valid onTranslated callback invoked")
    {
        When(Method(observer, onTranslated)).Do([](auto apiResponse){
            REQUIRE(apiResponse.translation == "Hallo");
        });

        translator.translate("Hello");

        Verify(Method(observer, onTranslated)).Once();
    }

    SECTION("Translator initialized with valid from and to languages")
    {
        REQUIRE(availableLanguages.contains(translator.sourceLanguage()));
        REQUIRE(availableLanguages.contains(translator.targetLanguage()));
        REQUIRE(translator.sourceLanguage() != translator.targetLanguage());
    }

    SECTION("Change source language")
    {
        translator.setSourceLanguage("ru");

        Verify(Method(observer, onLanguageChanged)).Once();
    }

    SECTION("Changing of source language to the same doesn't trigger notification")
    {
        translator.setSourceLanguage(translator.sourceLanguage());

        Verify(Method(observer, onLanguageChanged)).Never();
    }

    SECTION("Change desination language")
    {
        translator.setTargetLanguage("ru");

        Verify(Method(observer, onLanguageChanged)).Once();
    }

    SECTION("Changing of target language to the same doesn't trigger notification")
    {
        translator.setTargetLanguage(translator.targetLanguage());

        Verify(Method(observer, onLanguageChanged)).Never();
    }

    SECTION("Swap source and target languages")
    {
        const auto sourceLanguage = translator.sourceLanguage();
        const auto targetLanguage = translator.targetLanguage();

        SECTION("Assign source language to target language")
        {
            translator.setTargetLanguage(sourceLanguage);

            REQUIRE(translator.sourceLanguage() == targetLanguage);
            REQUIRE(translator.targetLanguage() == sourceLanguage);
        }

        SECTION("Assign target language to source language")
        {
            translator.setSourceLanguage(targetLanguage);

            REQUIRE(translator.sourceLanguage() == targetLanguage);
            REQUIRE(translator.targetLanguage() == sourceLanguage);
        }
    }
}
