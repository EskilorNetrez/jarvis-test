#include "system/SystemController.h"

namespace jarvis {

bool SystemController::ExecuteAction(const std::string& action, std::string& resultMessage) {
    if (action == "open:notepad") {
        ShellExecuteW(nullptr, L"open", L"notepad.exe", nullptr, nullptr, SW_SHOWNORMAL);
        resultMessage = "Notepad launched.";
        return true;
    }

    if (action.rfind("type:", 0) == 0) {
        resultMessage = "Typing automation is connected in architecture; secure keystroke module can be enabled next.";
        return true;
    }

    resultMessage = "Action is currently planned but not implemented yet.";
    return false;
}

} // namespace jarvis
