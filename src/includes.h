#pragma once

#include <windows.h>
#include <wrl.h>

#include <WebView2.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

using Microsoft::WRL::Callback;
using Microsoft::WRL::ComPtr;

namespace jarvis {

enum class UiState {
    Idle,
    Listening,
    Thinking,
    Speaking,
    Confirming,
    Error
};

inline std::wstring ToWide(const std::string& input) {
    if (input.empty()) {
        return {};
    }
    const int len = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, nullptr, 0);
    std::wstring output(len - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, output.data(), len);
    return output;
}

inline std::string ToUtf8(const std::wstring& input) {
    if (input.empty()) {
        return {};
    }
    const int len = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string output(len - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, output.data(), len, nullptr, nullptr);
    return output;
}

} // namespace jarvis
