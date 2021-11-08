#include "ApiUrlProvider.h"

#include <catch.hpp>


TEST_CASE("Api URL is valid")
{
    const auto apiUrlProvider = ApiUrlProvider();

    const auto apiUrl = apiUrlProvider.apiUrl();

    REQUIRE(!apiUrl.isEmpty());
    REQUIRE((apiUrl.startsWith("http://") || apiUrl.startsWith("https://")));
}
