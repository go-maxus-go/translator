#include "NetworkManager.h"

#include <catch.hpp>

#include <QCoreApplication>


TEST_CASE("Integration Test. Receive a responce after query is sent")
{
    int argc = 0;
    char* argv[0];
    QCoreApplication app(argc, argv);

    auto networkManager = NetworkManager();
    REQUIRE(networkManager.activeQueryCount() == 0);

    const auto googleQuery = "https://www.google.com";

    auto isResponceCalled = false;
    auto responseCallback =
        [&isResponceCalled, googleQuery](auto response, auto query) {
            isResponceCalled = true;
            REQUIRE(googleQuery == query);
            REQUIRE(!response.isEmpty());
        };

    networkManager.sendQuery(googleQuery, responseCallback);

    REQUIRE(networkManager.activeQueryCount() == 1);

    while (networkManager.activeQueryCount() != 0)
        app.processEvents();

    REQUIRE(isResponceCalled);
}

TEST_CASE("Integration test. Receive an error if the query is not valid")
{
    int argc = 0;
    char* argv[0];
    QCoreApplication app(argc, argv);

    auto networkManager = NetworkManager();

    const auto invalidQuery = "invalid.query";

    auto responseCallback = [](auto&&...) { REQUIRE(false); };

    auto errorCallbackCalled = false;
    auto errorCallback = [&invalidQuery, &errorCallbackCalled](auto&& query) {
        errorCallbackCalled = true;
        REQUIRE(invalidQuery == query);
    };

    networkManager.sendQuery(invalidQuery, responseCallback, errorCallback);

    REQUIRE(networkManager.activeQueryCount() == 1);

    while (networkManager.activeQueryCount() != 0)
        app.processEvents();

    REQUIRE(errorCallbackCalled);
}
