#pragma once

#include "ai/AssistantEngine.h"
#include "core/EventBus.h"
#include "includes.h"
#include "ui/WebViewHost.h"

namespace jarvis {

class App {
public:
    App();
    ~App();

    bool Initialize(HINSTANCE instance, int showCmd);
    int Run();

private:
    EventBus bus_;
    WebViewHost ui_;
    AssistantEngine engine_;
};

} // namespace jarvis
