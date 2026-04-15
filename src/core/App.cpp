#include "core/App.h"

namespace jarvis {

App::App() : ui_(bus_), engine_(bus_) {
}

App::~App() {
    engine_.Stop();
}

bool App::Initialize(HINSTANCE instance, int showCmd) {
    if (!ui_.Create(instance, showCmd)) {
        return false;
    }

    if (!engine_.Start()) {
        return false;
    }

    return true;
}

int App::Run() {
    return ui_.RunMessageLoop();
}

} // namespace jarvis
