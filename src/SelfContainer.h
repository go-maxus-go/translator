#pragma once

#include <memory>
#include <functional>
#include <unordered_set>


template<class Base>
class SelfContainer
{
public:
    SelfContainer(Base* base): m_self{base, [](auto&&){}}
    {}

protected:
    std::shared_ptr<Base> self() const
    {
        return m_self;
    }

private:
    std::shared_ptr<Base> m_self;
};
