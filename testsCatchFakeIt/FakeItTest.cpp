#include <string>

#include <catch.hpp>

#include <fakeit.hpp>


using namespace fakeit;

TEST_CASE("FakeIt", "[FI]")
{
    struct SomeInterface
    {
        virtual ~SomeInterface() = 0;

        virtual int foo(int) = 0;
        virtual int bar(int, int) = 0;

        virtual int mul(int a, int b) { return a * b; }
        virtual int sum(int a, int b) { return a + b; }
    };

    Mock<SomeInterface> mock;

    SECTION("Returning a value without checking the input")
    {
        SECTION("Stub a method to return a value once")
        {
            When(Method(mock,foo)).Return(1);

            SomeInterface& object = mock.get();

            REQUIRE(object.foo(42) == 1);
        }

        SECTION("Stub multiple return values")
        {
            When(Method(mock,foo)).Return(1,2,3);
            // Or When(Method(mock,foo)).Return(1).Return(2).Return(3);

            SomeInterface& object = mock();

            REQUIRE(object.foo(42) == 1);
            REQUIRE(object.foo(42) == 2);
            REQUIRE(object.foo(42) == 3);
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

        SECTION("Just fake a method")
        {
            Fake(Method(mock,foo));
            mock().foo(42);
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

        SECTION("Stub foo to return 1 only when arg > 1")
        {
            Method(mock,foo) = 0;
            When(Method(mock,foo).Using(Gt(1))).Return(1);
            // Gt, Ge, Lt, Le, Ne, Eq

            REQUIRE(mock().foo(0) == 0);
            REQUIRE(mock().foo(1) == 0);
            REQUIRE(mock().foo(2) == 1);
        }

        SECTION("Stub bar to return 1 only when the second arg <= 1")
        {
            Method(mock,bar) = 0;
            When(Method(mock,bar).Using(_, Le(1))).AlwaysReturn(1);

            REQUIRE(mock().bar(0, 0) == 1);
            REQUIRE(mock().bar(0, 1) == 1);
            REQUIRE(mock().bar(0, 2) == 0);
        }

        SECTION("Stub foo to return 0 when arg is odd, otherwise to return 1")
        {
            Method(mock,foo) = 0;
            auto oddMatcher = [](int arg){ return arg % 2 != 0; };
            When(Method(mock,foo).Matching(oddMatcher)).Return(1);

            REQUIRE(mock().foo(42) == 0);
            REQUIRE(mock().foo(43) == 1);
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

        SECTION("Fake destructor")
        {
            When(Dtor(mock)).Do([](){ throw std::exception(); });

            REQUIRE_THROWS_AS(mock().~SomeInterface(), std::exception);
        }
    }

    SECTION("Verification of mock invocation")
    {
        Method(mock,foo) = 0;
        Method(mock,bar) = 0;

        SomeInterface& object = mock.get();

        object.foo(1);
        object.foo(2);
        object.foo(3);
        object.bar(4,5);

        SECTION("Verify foo was invoked at least once")
        {
            Verify(Method(mock,foo));
            Verify(Method(mock,foo)).AtLeastOnce();
            Verify(Method(mock,foo)).AtLeast(1);
            Verify(Method(mock,foo)).AtLeast(1_Time);
        }

        SECTION("Verify foo was invoked exactly 3 times")
        {
            Verify(Method(mock,foo)).Exactly(3);
            Verify(Method(mock,foo)).Exactly(3_Times);
        }

        SECTION("Verify foo(1) was invoked exactly once")
        {
            Verify(Method(mock,foo).Using(1)).Once();
            Verify(Method(mock,foo).Using(1)).Exactly(1);
            Verify(Method(mock,foo).Using(1)).Exactly(Once);
        }

        SECTION("Verify bar(a>b) was invoked exactly once")
        {
            Verify(Method(mock,bar).
                Matching([](int a, int b){ return a > b; })).
                Never();

            Verify(Method(mock,bar).
                Matching([](int a, int b){ return a < b; })).
                Exactly(Once);
        }

        SECTION("Verify all invokations")
        {
            Verify(Method(mock,foo)).Exactly(3);
            Verify(Method(mock,bar)).AtLeastOnce();
            VerifyNoOtherInvocations(mock);
        }
    }

    SECTION("Spy implemented methods invocation")
    {
        Method(mock,mul) = 10;
        Spy(Method(mock,sum));

        REQUIRE(mock().mul(1,2) == 10);
        REQUIRE(mock().sum(1,2) == 3);

        Verify(Method(mock,mul)).Once();
        Verify(Method(mock,sum)).Once();
    }
}
