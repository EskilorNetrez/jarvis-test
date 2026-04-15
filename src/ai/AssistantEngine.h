#pragma once

#include "ai/LlmEngine.h"
#include "ai/TtsEngine.h"
#include "ai/VoiceEngine.h"
#include "core/EventBus.h"
#include "includes.h"
#include "system/ScreenContext.h"
#include "system/SystemController.h"

namespace jarvis {

class AssistantEngine {
public:
    explicit AssistantEngine(EventBus& bus);

    bool Start();
    void Stop();

private:
    void OnWakeWord();
    void OnTranscript(const std::string& transcript);

    EventBus& bus_;
    VoiceEngine voice_;
    LlmEngine llm_;
    TtsEngine tts_;
    ScreenContext screen_;
    SystemController system_;
};

} // namespace jarvis
