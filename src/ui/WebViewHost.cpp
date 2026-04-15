#include "ui/WebViewHost.h"

#include <format>

namespace jarvis {

namespace {
constexpr wchar_t kWindowClassName[] = L"JarvisDesktopWindow";
}

WebViewHost::WebViewHost(EventBus& bus) : bus_(bus) {
    busToken_ = bus_.Subscribe([this](const UiEvent& event) {
        SendEvent(event);
    });
}

WebViewHost::~WebViewHost() {
    bus_.Unsubscribe(busToken_);
}

bool WebViewHost::Create(HINSTANCE instance, int showCmd) {
    instance_ = instance;

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance_;
    wc.lpszClassName = kWindowClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    RegisterClassExW(&wc);

    hwnd_ = CreateWindowExW(
        0,
        kWindowClassName,
        L"JARVIS Local Assistant",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        860,
        nullptr,
        nullptr,
        instance_,
        this);

    if (!hwnd_) {
        return false;
    }

    ShowWindow(hwnd_, showCmd);
    UpdateWindow(hwnd_);
    InitializeWebView();
    return true;
}

int WebViewHost::RunMessageLoop() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

void WebViewHost::InitializeWebView() {
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) {
                    MessageBoxW(hwnd_, L"Failed to initialize WebView2 runtime.", L"Error", MB_ICONERROR);
                    return result;
                }
                environment_ = env;

                environment_->CreateCoreWebView2Controller(hwnd_,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT controllerResult, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(controllerResult)) {
                                MessageBoxW(hwnd_, L"Failed to create WebView2 controller.", L"Error", MB_ICONERROR);
                                return controllerResult;
                            }

                            controller_ = controller;
                            controller_->get_CoreWebView2(&webview_);

                            RECT bounds;
                            GetClientRect(hwnd_, &bounds);
                            controller_->put_Bounds(bounds);

                            std::wstring assetsPath = std::filesystem::absolute("frontend").wstring();
                            webview_->SetVirtualHostNameToFolderMapping(
                                L"appassets.local",
                                assetsPath.c_str(),
                                COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

                            webview_->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [this](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                        LPWSTR message = nullptr;
                                        if (SUCCEEDED(args->TryGetWebMessageAsString(&message)) && message != nullptr) {
                                            HandleWebMessage(ToUtf8(message));
                                            CoTaskMemFree(message);
                                        }
                                        return S_OK;
                                    }).Get(),
                                nullptr);

                            webview_->Navigate(L"https://appassets.local/index.html");
                            return S_OK;
                        }).Get());
                return S_OK;
            }).Get());
}

void WebViewHost::HandleWebMessage(const std::string& json) {
    if (json == "confirm_action") {
        bus_.Publish(UiEvent{.state = UiState::Thinking, .transcript = "User confirmed action from UI"});
    }
}

void WebViewHost::PostJsonToUi(const std::string& json) {
    if (webview_) {
        webview_->PostWebMessageAsString(ToWide(json).c_str());
    }
}

void WebViewHost::SendEvent(const UiEvent& event) {
    std::string state = "idle";
    switch (event.state) {
    case UiState::Idle: state = "idle"; break;
    case UiState::Listening: state = "listening"; break;
    case UiState::Thinking: state = "thinking"; break;
    case UiState::Speaking: state = "speaking"; break;
    case UiState::Confirming: state = "confirming"; break;
    case UiState::Error: state = "error"; break;
    }

    const std::string payload = std::format(
        R"({{"type":"ui_event","state":"{}","transcript":"{}","response":"{}","audioLevel":{}}})",
        state,
        event.transcript,
        event.response,
        event.audioLevel);
    PostJsonToUi(payload);
}

LRESULT CALLBACK WebViewHost::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    WebViewHost* self = nullptr;

    if (message == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = static_cast<WebViewHost*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    }
    else {
        self = reinterpret_cast<WebViewHost*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self && self->controller_ && message == WM_SIZE) {
        RECT bounds;
        GetClientRect(hwnd, &bounds);
        self->controller_->put_Bounds(bounds);
    }

    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

} // namespace jarvis
