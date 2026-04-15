#pragma once

#include "includes.h"

namespace jarvis {

class TtsEngine {
public:
    TtsEngine() = default;
    ~TtsEngine();

    bool Initialize();
    void Speak(const std::string& text, const std::wstring& localeHint = L"en-US");

private:
    ISpVoice* voice_{nullptr};
};

} // namespace jarvis
