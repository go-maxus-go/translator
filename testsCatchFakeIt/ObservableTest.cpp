#include "Observervable.h"

#include <memory>

#include <catch.hpp>
#include <fakeit.hpp>


using namespace fakeit;

struct IObserver
{
    virtual ~IObserver() = default;

    virtual void foo() = 0;
};

TEST_CASE("Observable attaches, detaches and notifies observers")
{
    Mock<IObserver> observer1;
    Fake(Method(observer1, foo));
    auto observerPtr1 = std::shared_ptr<IObserver>{&observer1(), [](auto&&){}};

    Mock<IObserver> observer2;
    Fake(Method(observer2, foo));
    auto observerPtr2 = std::shared_ptr<IObserver>{&observer2(), [](auto&&){}};

    Observable<IObserver> observable;

    SECTION("Attach first observer")
    {
        observable.attach(observerPtr1);

        SECTION("Attached observer receives notifications")
        {
            observable.notify([](auto&& obs){ obs.foo(); });

            Verify(Method(observer1, foo)).Once();
        }

        SECTION("Attach second observer")
        {
            observable.attach(observerPtr2);

            SECTION("All attached observers receive notification")
            {
                observable.notify([](auto&& obs){ obs.foo(); });

                Verify(Method(observer1, foo)).Once();
                Verify(Method(observer2, foo)).Once();
            }

            SECTION("Detached observer doesn't receive notifications")
            {
                observable.detach(observerPtr1);

                observable.notify([](auto&& obs){ obs.foo(); });

                Verify(Method(observer1, foo)).Never();
                Verify(Method(observer2, foo)).Once();
            }

            SECTION("Destroyed observers do not receive notifications")
            {
                observerPtr1.reset();

                observable.notify([](auto&& obs){ obs.foo(); });

                Verify(Method(observer1, foo)).Never();
                Verify(Method(observer2, foo)).Once();
            }
        }
    }

}
