#pragma once

#include "includes.h"

namespace jarvis {

class VoiceEngine {
public:
    struct Callback {
        std::function<void()> onWake;
        std::function<void(const std::string& text)> onTranscript;
        std::function<void(float level)> onAudioLevel;
    };

    VoiceEngine() = default;
    ~VoiceEngine();

    bool Start(Callback callback);
    void Stop();

private:
    void CaptureLoop();

    std::atomic<bool> running_{false};
    std::thread worker_;
    Callback callback_;
};

} // namespace jarvis
