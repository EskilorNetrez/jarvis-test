#include "core/EventBus.h"

namespace jarvis {

int EventBus::Subscribe(Handler handler) {
    std::scoped_lock lock(mutex_);
    const int token = nextToken_++;
    handlers_.emplace(token, std::move(handler));
    return token;
}

void EventBus::Unsubscribe(int token) {
    std::scoped_lock lock(mutex_);
    handlers_.erase(token);
}

void EventBus::Publish(const UiEvent& event) {
    std::vector<Handler> copy;
    {
        std::scoped_lock lock(mutex_);
        copy.reserve(handlers_.size());
        for (const auto& [_, handler] : handlers_) {
            copy.push_back(handler);
        }
    }

    for (const auto& handler : copy) {
        handler(event);
    }
}

} // namespace jarvis
