#pragma once

#include "includes.h"

namespace jarvis {

struct UiEvent {
    UiState state{UiState::Idle};
    std::string transcript;
    std::string response;
    float audioLevel{0.0F};
};

class EventBus {
public:
    using Handler = std::function<void(const UiEvent&)>;

    int Subscribe(Handler handler);
    void Unsubscribe(int token);
    void Publish(const UiEvent& event);

private:
    std::mutex mutex_;
    int nextToken_{1};
    std::unordered_map<int, Handler> handlers_;
};

} // namespace jarvis
