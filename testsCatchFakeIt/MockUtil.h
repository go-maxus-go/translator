#pragma once

#include <memory>
#include <type_traits>


template<class MockType>
auto mock2ptr(MockType&& mock)
{
    using InterfaceType = std::remove_reference_t<decltype(mock())>;
    return std::shared_ptr<InterfaceType>{&mock(), [](auto&&){}};
}
