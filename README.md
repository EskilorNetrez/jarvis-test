# JARVIS Local Assistant (Windows, C++ + WebView2)

A fully-local desktop AI assistant starter inspired by JARVIS, with a native C++ orchestration engine and a modern WebView2 front-end featuring a reactive circular particle system.

## Highlights

- **Native C++ backend** for orchestration, automation, and integration points.
- **Embedded HTML/CSS/JS UI** rendered inside **Edge WebView2**.
- **Voice-first architecture** with wake-word + transcript callback flow.
- **Local LLM integration hook** (llama.cpp style `llama-cli.exe`).
- **Local TTS** via Microsoft SAPI (`ISpVoice`).
- **Safety first**: risky actions route through confirmation stage.

## Project Structure

```text
jarvis-test/
├─ CMakeLists.txt
├─ README.md
├─ frontend/
│  ├─ index.html
│  ├─ style.css
│  ├─ particles.js
│  └─ app.js
└─ src/
   ├─ includes.h
   ├─ main.cpp
   ├─ core/
   │  ├─ App.h / App.cpp
   │  └─ EventBus.h / EventBus.cpp
   ├─ ai/
   │  ├─ AssistantEngine.h / AssistantEngine.cpp
   │  ├─ VoiceEngine.h / VoiceEngine.cpp
   │  ├─ LlmEngine.h / LlmEngine.cpp
   │  └─ TtsEngine.h / TtsEngine.cpp
   ├─ system/
   │  ├─ SystemController.h / SystemController.cpp
   │  └─ ScreenContext.h / ScreenContext.cpp
   └─ ui/
      ├─ WebViewHost.h
      └─ WebViewHost.cpp
```

## Build (Visual Studio 2026, Release x64)

### 1) Prerequisites

1. **Visual Studio 2026** with Desktop development for C++.
2. **CMake 3.24+** (bundled with VS is fine).
3. **Microsoft Edge WebView2 SDK**:
   - Preferred: install NuGet/CMake package `Microsoft.Web.WebView2`.
   - Alternative: set `WEBVIEW2_SDK_PATH` to the SDK root with `build/native/include` and `build/native/x64`.
4. (Optional for AI) local model runtime:
   - `bin/llama-cli.exe`
   - `models/tinyllama.gguf`

### 2) Configure and build

```powershell
cd <repo>
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
```

### 3) Run

```powershell
.\build\Release\LocalJarvis.exe
```

## Runtime Flow

1. `VoiceEngine` emits wake + transcript + audio level events.
2. `AssistantEngine` gathers screen context and sends user utterance to `LlmEngine`.
3. Decision can request confirmation for critical actions.
4. `SystemController` executes approved actions.
5. `TtsEngine` speaks the response while UI state changes (`idle/listening/thinking/speaking/confirming`).

## Integrating Real Local Voice + LLM

- Replace `VoiceEngine::CaptureLoop()` with a real microphone pipeline:
  - wake-word detector for “JARVIS” / “Hey JARVIS”
  - local STT (Whisper.cpp / Vosk / Win speech)
- Expand `LlmEngine::Interpret()` to parse strict JSON tool plans from local model.
- Add OCR + desktop-duplication in `ScreenContext`.
- Expand `SystemController` for secure mouse/keyboard/browser automation with per-action policy prompts.

## Security Notes

- Keep an explicit allowlist for automation tools.
- For file deletion, process killing, registry changes, and shell execution: require a clear verbal + UI confirmation.
- Log every action and decision to local audit files.

