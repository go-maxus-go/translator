#pragma once

#include <memory>
#include <functional>
#include <unordered_set>
#include <set>


template<class Observer>
class Observable
{
    using ObserverPtr = std::weak_ptr<Observer>;
public:
    void attach(ObserverPtr observer)
    {
        m_observers.push_back(observer);
    }

    void detach(ObserverPtr observer)
    {
        auto it = std::remove_if(
            m_observers.begin(),
            m_observers.end(),
            [&observer](auto&& obs) {
                return !observer.owner_before(obs) &&
                    !obs.owner_before(observer);
            }
        );
        m_observers.erase(it, m_observers.end());
    }

    void notify(std::function<void(Observer&)> notification)
    {
        auto observers = m_observers;
        for (auto&& observer : observers)
        {
            if (auto ptr = observer.lock())
                notification(*ptr);
        }

        cleanUp();
    }

private:
    void cleanUp()
    {
        auto it = std::remove_if(
            m_observers.begin(),
            m_observers.end(),
            [](auto&& obs){ return obs.expired(); }
        );
        m_observers.erase(it, m_observers.end());
    }

private:
    std::vector<ObserverPtr> m_observers;
};
