#include "SelfContainer.h"

#include <catch.hpp>


TEST_CASE("Self container provides 'this' pointer")
{
    struct Dummy : SelfContainer<Dummy>
    {
        Dummy() : SelfContainer<Dummy>{this}
        {}

        using SelfContainer<Dummy>::self;
    };

    Dummy dummy;

    const auto self = dummy.self();

    REQUIRE(self.get() == &dummy);
}
