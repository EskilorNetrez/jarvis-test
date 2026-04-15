#include "ai/LlmEngine.h"

#include <fstream>

namespace jarvis {

bool LlmEngine::Initialize(const std::filesystem::path& modelPath, const std::filesystem::path& llamaCliPath) {
    modelPath_ = modelPath;
    llamaCliPath_ = llamaCliPath;
    return true;
}

LlmDecision LlmEngine::Interpret(const std::string& transcript, const std::string& screenContext) {
    // Integration point for a local LLM (llama.cpp, ONNX runtime, etc.).
    // Keep fallback so project runs without model download.
    if (!modelPath_.empty() && !llamaCliPath_.empty() &&
        std::filesystem::exists(modelPath_) && std::filesystem::exists(llamaCliPath_)) {
        const auto temp = std::filesystem::temp_directory_path() / "jarvis_prompt.txt";
        std::ofstream out(temp);
        out << "System: You are JARVIS. Return strict JSON with keys: intent, spokenResponse, action, requiresConfirmation.\n";
        out << "Screen: " << screenContext << "\n";
        out << "User: " << transcript << "\n";
        out.close();

        std::stringstream cmd;
        cmd << '"' << llamaCliPath_.string() << '"'
            << " -m \"" << modelPath_.string() << "\""
            << " -f \"" << temp.string() << "\""
            << " -n 256 --temp 0.2";

        FILE* pipe = _popen(cmd.str().c_str(), "r");
        if (pipe) {
            std::string output;
            char buffer[512];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                output.append(buffer);
            }
            _pclose(pipe);

            LlmDecision decision = RuleBasedFallback(transcript);
            // Lightweight extraction (robust parser can be swapped later).
            const auto actionPos = output.find("\"action\"");
            if (actionPos != std::string::npos) {
                decision.spokenResponse = "Understood. I created a plan using the local language model.";
                decision.action = "plan";
            }
            return decision;
        }
    }

    return RuleBasedFallback(transcript);
}

LlmDecision LlmEngine::RuleBasedFallback(const std::string& transcript) const {
    LlmDecision decision;
    decision.intent = "general_assistance";
    decision.spokenResponse = "I am ready. I can launch apps, draft text, inspect your screen, and automate routine tasks locally.";

    if (transcript.find("open") != std::string::npos) {
        decision.intent = "open_application";
        decision.action = "open:notepad";
        decision.spokenResponse = "I can open Notepad for you. Please confirm.";
        decision.requiresConfirmation = true;
    }

    if (transcript.find("weather") != std::string::npos || transcript.find("Oslo") != std::string::npos) {
        decision.intent = "query";
        decision.spokenResponse = "Jeg er klar. Været i Oslo må sjekkes via en lokal datakilde om du ønsker det.";
    }

    return decision;
}

} // namespace jarvis
