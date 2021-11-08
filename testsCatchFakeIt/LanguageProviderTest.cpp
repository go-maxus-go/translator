#include "LanguageProvider.h"

#include <catch2/catch.hpp>


TEST_CASE("List of available languages is not empty")
{
    const auto languageProvider = LanguageProvider();

    const auto languages = languageProvider.availableLanguages();

    REQUIRE(!languages.isEmpty());
}

TEST_CASE("Available languages are valid")
{
    const auto languageProvider = LanguageProvider();

    const auto languages = languageProvider.availableLanguages();

    for (const auto& language: languages)
        REQUIRE(language.length() == 2);
}
