#include "TranslationQueryBuilder.h"

#include <catch.hpp>
#include <fakeit.hpp>

#include "MockUtil.h"

#include "ApiUrlProvider.h"


using namespace fakeit;

TEST_CASE("Translation query is valid. Classic style.")
{
    const auto apiUrlProvider = std::make_shared<ApiUrlProvider>();
    const auto builder = TranslationQueryBuilder(apiUrlProvider);

    const auto fromLanguage = "fromLanguage";
    const auto toLanguage = "toLanguage";
    const auto text = "UniqueText";
    const auto query = builder.build(fromLanguage, toLanguage, text);

    REQUIRE(!query.isEmpty());
    REQUIRE(query.startsWith(apiUrlProvider->apiUrl()));
    REQUIRE(query.contains(fromLanguage));
    REQUIRE(query.contains(toLanguage));
    REQUIRE(query.contains(text));
}

TEST_CASE("Translation query is valid. London style.")
{
    const auto dummyApi = "dummyApi/";
    Mock<IApiUrlProvider> apiUrlProviderMock;
    When(Method(apiUrlProviderMock, apiUrl)).Return(dummyApi);

    const auto apiUrlProvider = mock2ptr(apiUrlProviderMock);
    const auto builder = TranslationQueryBuilder(apiUrlProvider);

    const auto fromLanguage = "fromLanguage";
    const auto toLanguage = "toLanguage";
    const auto text = "UniqueText";
    const auto query = builder.build(fromLanguage, toLanguage, text);

    REQUIRE(!query.isEmpty());
    REQUIRE(query.startsWith(dummyApi));
    REQUIRE(query.contains(fromLanguage));
    REQUIRE(query.contains(toLanguage));
    REQUIRE(query.contains(text));
}
