#include "ai/AssistantEngine.h"

namespace jarvis {

AssistantEngine::AssistantEngine(EventBus& bus) : bus_(bus) {
}

bool AssistantEngine::Start() {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    const auto modelPath = std::filesystem::path(L"models/tinyllama.gguf");
    const auto llamaCliPath = std::filesystem::path(L"bin/llama-cli.exe");
    llm_.Initialize(modelPath, llamaCliPath);
    tts_.Initialize();

    return voice_.Start({
        .onWake = [this]() { OnWakeWord(); },
        .onTranscript = [this](const std::string& text) { OnTranscript(text); },
        .onAudioLevel = [this](float level) {
            bus_.Publish(UiEvent{.state = UiState::Listening, .audioLevel = level});
        }
    });
}

void AssistantEngine::Stop() {
    voice_.Stop();
    CoUninitialize();
}

void AssistantEngine::OnWakeWord() {
    bus_.Publish(UiEvent{.state = UiState::Listening, .transcript = "Wake word detected: Hey JARVIS"});
}

void AssistantEngine::OnTranscript(const std::string& transcript) {
    bus_.Publish(UiEvent{.state = UiState::Thinking, .transcript = transcript});

    const std::string screenContext = screen_.CaptureAndSummarize();
    const LlmDecision decision = llm_.Interpret(transcript, screenContext);

    if (decision.requiresConfirmation) {
        bus_.Publish(UiEvent{.state = UiState::Confirming, .response = decision.spokenResponse});
        tts_.Speak(decision.spokenResponse);

        std::string result;
        const bool ok = system_.ExecuteAction(decision.action, result);
        bus_.Publish(UiEvent{.state = ok ? UiState::Speaking : UiState::Error, .response = result});
        tts_.Speak(result);
        return;
    }

    bus_.Publish(UiEvent{.state = UiState::Speaking, .response = decision.spokenResponse});
    tts_.Speak(decision.spokenResponse);
    bus_.Publish(UiEvent{.state = UiState::Idle});
}

} // namespace jarvis
