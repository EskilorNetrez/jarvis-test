#pragma once

#include "includes.h"

namespace jarvis {

struct LlmDecision {
    std::string intent;
    std::string spokenResponse;
    std::string action;
    bool requiresConfirmation{false};
};

class LlmEngine {
public:
    bool Initialize(const std::filesystem::path& modelPath, const std::filesystem::path& llamaCliPath);
    LlmDecision Interpret(const std::string& transcript, const std::string& screenContext);

private:
    std::filesystem::path modelPath_;
    std::filesystem::path llamaCliPath_;

    LlmDecision RuleBasedFallback(const std::string& transcript) const;
};

} // namespace jarvis
