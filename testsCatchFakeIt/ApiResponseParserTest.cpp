#include "ApiResponseParser.h"

#include <catch.hpp>


TEST_CASE("Parsing of a valid json gives a valid result")
{
    const auto parser = ApiResponseParser();

    const auto response = R"(
        {
          "matches": [
            {
              "segment": "How are you?\n\nBad",
              "translation": "Wie geht es Ihnen?&#10;&#10; Schlecht",
              "source": "en-GB",
              "target": "de-DE"
            }
          ]
        }
    )";

    auto apiResponce = parser.parse(response);

    REQUIRE(apiResponce.languageFrom == "en");
    REQUIRE(apiResponce.languageTo == "de");
    REQUIRE(apiResponce.source == "How are you?\n\nBad");
    REQUIRE(apiResponce.translation == "Wie geht es Ihnen?\n\nSchlecht");
}

TEST_CASE("Parsing of an invalid json throws an exception")
{
    const auto parser = ApiResponseParser();

    const auto response = R"(
        {
            Invaild json
        }
    )";

    REQUIRE_THROWS(parser.parse(response));
}
