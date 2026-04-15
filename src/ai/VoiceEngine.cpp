#include "ai/VoiceEngine.h"

#include <cmath>

namespace jarvis {

VoiceEngine::~VoiceEngine() {
    Stop();
}

bool VoiceEngine::Start(Callback callback) {
    if (running_) {
        return true;
    }
    callback_ = std::move(callback);
    running_ = true;
    worker_ = std::thread(&VoiceEngine::CaptureLoop, this);
    return true;
}

void VoiceEngine::Stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void VoiceEngine::CaptureLoop() {
    // Placeholder local voice pipeline.
    // Production: wire Whisper + wake-word model + microphone stream here.
    using namespace std::chrono_literals;
    auto start = std::chrono::steady_clock::now();

    while (running_) {
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();
        const float level = 0.2F + 0.15F * std::sin(static_cast<float>(ms) / 200.0F);
        if (callback_.onAudioLevel) {
            callback_.onAudioLevel(level);
        }

        // Demo interaction every ~12 seconds to validate full end-to-end wiring.
        if ((ms % 12000) < 80) {
            if (callback_.onWake) {
                callback_.onWake();
            }
            if (callback_.onTranscript) {
                callback_.onTranscript(
                    "Hey JARVIS, open notepad and then tell me the weather in Oslo in Norwegian.");
            }
        }

        std::this_thread::sleep_for(40ms);
    }
}

} // namespace jarvis
