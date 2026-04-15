#include "core/App.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    jarvis::App app;
    if (!app.Initialize(hInstance, nCmdShow)) {
        MessageBoxW(nullptr, L"Failed to initialize JARVIS Local Assistant.", L"Startup Error", MB_ICONERROR);
        return -1;
    }

    return app.Run();
}
