#pragma once

#include "core/EventBus.h"
#include "includes.h"

namespace jarvis {

class WebViewHost {
public:
    explicit WebViewHost(EventBus& bus);
    ~WebViewHost();

    bool Create(HINSTANCE instance, int showCmd);
    int RunMessageLoop();
    void SendEvent(const UiEvent& event);

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    void InitializeWebView();
    void HandleWebMessage(const std::string& json);
    void PostJsonToUi(const std::string& json);

    EventBus& bus_;
    int busToken_{0};

    HWND hwnd_{nullptr};
    HINSTANCE instance_{nullptr};

    ComPtr<ICoreWebView2Environment> environment_;
    ComPtr<ICoreWebView2Controller> controller_;
    ComPtr<ICoreWebView2> webview_;
};

} // namespace jarvis
