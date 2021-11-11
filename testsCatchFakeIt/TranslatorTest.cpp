#include "Translator.h"

#include <catch.hpp>
#include <fakeit.hpp>


using namespace fakeit;

TEST_CASE("Full Translator test")
{
    const auto availableLanguages = QStringList{"de", "en", "ru"};
    Mock<ILanguageProvider> languageProvider;
    const auto languageProviderPtr = std::shared_ptr<ILanguageProvider>(
            &languageProvider(), [](auto){});
    When(Method(languageProvider, availableLanguages)).
        Return(availableLanguages);

    Mock<ITranslatorObserver> observerMock;
    const auto observer = std::shared_ptr<ITranslatorObserver>(
        &observerMock(), [](auto){});
    Fake(Method(observerMock, onLanguageChanged));

    auto translator = Translator(languageProviderPtr);
    translator.attach(observer);

    SECTION("Translator initialized with valid from and to languages")
    {
        REQUIRE(availableLanguages.contains(translator.languageFrom()));
        REQUIRE(availableLanguages.contains(translator.languageTo()));
        REQUIRE(translator.languageFrom() != translator.languageTo());
    }

    SECTION("Change source language")
    {
        translator.setLanguageFrom("de");

        Verify(Method(observerMock, onLanguageChanged)).Once();
    }

    SECTION("Change desination language")
    {
        translator.setLanguageTo("ru");

        Verify(Method(observerMock, onLanguageChanged)).Once();
    }

    SECTION("Swap source and destination languages")
    {
        const auto sourceLang = translator.languageFrom();
        const auto destinationLang = translator.languageTo();

        SECTION("Assign source language to destination language")
        {
            translator.setLanguageTo(sourceLang);

            REQUIRE(translator.languageFrom() == destinationLang);
            REQUIRE(translator.languageTo() == sourceLang);
        }

        SECTION("Assign destination language to source language")
        {
            translator.setLanguageFrom(destinationLang);

            REQUIRE(translator.languageFrom() == destinationLang);
            REQUIRE(translator.languageTo() == sourceLang);
        }
    }
}
