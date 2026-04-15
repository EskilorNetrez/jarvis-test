#include "ai/TtsEngine.h"

#include <sapi.h>

namespace jarvis {

TtsEngine::~TtsEngine() {
    if (voice_ != nullptr) {
        voice_->Release();
        voice_ = nullptr;
    }
}

bool TtsEngine::Initialize() {
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice,
        reinterpret_cast<void**>(&voice_));
    return SUCCEEDED(hr);
}

void TtsEngine::Speak(const std::string& text, const std::wstring& /*localeHint*/) {
    if (voice_ == nullptr) {
        return;
    }

    std::wstring wide = ToWide(text);
    voice_->Speak(wide.c_str(), SPF_ASYNC, nullptr);
}

} // namespace jarvis
