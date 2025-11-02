#pragma once
#include <functional>

namespace CE
{
    #define BIND_FUNC(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }

    template <class Owner, typename... Args>
    class Event {

    friend Owner;

    public:
        Event() = default;

        using Callback = typename std::function<void(Args...)>;
        using CallbackId = typename std::list<Callback>::iterator;

        CallbackId addCallback(Callback callback)
        {
            callbackList.push_back(callback);
            return --callbackList.end();
        }

        void removeCallback(CallbackId id)
        {
            callbackList.erase(id);
        }

    private:
        void broadcast(Args... args)
        {
            for (auto& callback : callbackList)
                callback(std::forward<Args>(args)...);
        }

        std::list<Callback> callbackList;
    };
}