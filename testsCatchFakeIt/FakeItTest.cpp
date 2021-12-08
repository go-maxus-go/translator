#include <string>

#include <catch.hpp>

#include <fakeit.hpp>


using namespace fakeit;

TEST_CASE("Stubbing")
{
    struct SomeInterface
    {
        virtual ~SomeInterface() = 0;

        virtual int foo(int) = 0;
    };

    Mock<SomeInterface> mock;

    SECTION("Returning a value without checking the input")
    {
        SECTION("Stub a method to return a value once")
        {
            When(Method(mock,foo)).Return(1);

            REQUIRE(mock().foo(42) == 1);
        }

        SECTION("Stub multiple return values")
        {
            When(Method(mock,foo)).Return(1,2,3);
            // Or When(Method(mock,foo)).Return(1).Return(2).Return(3);

            REQUIRE(mock().foo(42) == 1);
            REQUIRE(mock().foo(42) == 2);
            REQUIRE(mock().foo(42) == 3);
        }

        SECTION("Return the same value many times")
        {
            When(Method(mock,foo)).Return(5_Times(1));

            REQUIRE(mock().foo(42) == 1);
            REQUIRE(mock().foo(42) == 1);
            REQUIRE(mock().foo(42) == 1);
            REQUIRE(mock().foo(42) == 1);
            REQUIRE(mock().foo(42) == 1);
        }

        SECTION("Return many values many times")
        {
            When(Method(mock,foo)).Return(100_Times(1), 200_Times(2));

            for (int i = 0; i < 100; ++i)
                REQUIRE(mock().foo(42) == 1);
            for (int i = 0; i < 200; ++i)
                REQUIRE(mock().foo(42) == 2);
        }

        SECTION("Always return a value")
        {
            Method(mock,foo) = 1;
            // Or When(Method(mock,foo)).AlwaysReturn(1);

            REQUIRE(mock().foo(42) == 1);
            REQUIRE(mock().foo(42) == 1);
        }
    }

    SECTION("Returned value depends on the input")
    {
        SECTION("Stub foo(1) to return the value '100' once)")
        {
            When(Method(mock,foo).Using(1)).Return(100);
            // Or When(Method(mock,foo)(1)).Return(100);

            REQUIRE(mock().foo(1) == 100);
        }

        SECTION("Stub 'foo(1)' to always return '100'. For all other calls always return 0.")
        {
            Method(mock,foo) = 100;
            Method(mock,foo)(1) = 0;

            // Or
            // When(Method(mock,foo)).AlwaysReturn(0);
            // When(Method(mock,foo).Using(1)).AlwaysReturn(0);

            REQUIRE(mock().foo(42) == 100);
            REQUIRE(mock().foo(1) == 0);
        }
    }

    SECTION("A stub method throws an exception")
    {
        SECTION("A stub method throws an exception")
        {
            SECTION("Throw once")
            {
                When(Method(mock,foo)).Throw(std::exception());

                REQUIRE_THROWS_AS(mock().foo(42), std::exception);
            }

            SECTION("Throw several times")
            {
                When(Method(mock,foo)).Throw(std::exception(), std::exception());

                REQUIRE_THROWS_AS(mock().foo(42), std::exception);
                REQUIRE_THROWS_AS(mock().foo(42), std::exception);
            }

            SECTION("Throw many times")
            {
                When(Method(mock,foo)).Throw(100_Times(std::exception()));

                for (int i = 0; i < 100; ++i)
                    REQUIRE_THROWS_AS(mock().foo(42), std::exception);
            }

            SECTION("Always throw")
            {
                When(Method(mock,foo)).AlwaysThrow(std::exception());

                REQUIRE_THROWS_AS(mock().foo(42), std::exception);
                REQUIRE_THROWS_AS(mock().foo(42), std::exception);
            }
        }
    }

    SECTION("Fake method executes lambda expressions")
    {
        SECTION("Execute lambda once")
        {
            When(Method(mock,foo)).
                Do([](int a) { return a + a; }).
                Do([](int a) { return a * a; });

            REQUIRE(mock().foo(10) == 20);
            REQUIRE(mock().foo(10) == 100);
        }

        SECTION("Always execute lambda")
        {
            When(Method(mock,foo)).AlwaysDo([](int a) { return a + a; });

            REQUIRE(mock().foo(10) == 20);
            REQUIRE(mock().foo(10) == 20);
        }

        SECTION("Lambda throws an exception")
        {
            When(Method(mock,foo)).Do([](int)->int{ throw std::exception(); });

            REQUIRE_THROWS_AS(mock().foo(42), std::exception);
        }
    }

    SECTION("Fake destructor")
    {
        When(Dtor(mock)).Do([](){ throw std::exception(); });

        REQUIRE_THROWS_AS(mock().~SomeInterface(), std::exception);
    }
}
