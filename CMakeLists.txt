cmake_minimum_required(VERSION 3.24)
project(LocalJarvis VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_executable(LocalJarvis WIN32
    src/main.cpp
    src/core/App.cpp
    src/core/EventBus.cpp
    src/ai/AssistantEngine.cpp
    src/ai/LlmEngine.cpp
    src/ai/TtsEngine.cpp
    src/ai/VoiceEngine.cpp
    src/system/SystemController.cpp
    src/system/ScreenContext.cpp
    src/ui/WebViewHost.cpp
)

target_include_directories(LocalJarvis PRIVATE src)

target_compile_definitions(LocalJarvis PRIVATE
    UNICODE
    _UNICODE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

target_link_libraries(LocalJarvis PRIVATE
    ole32
    oleaut32
    user32
    gdi32
    shlwapi
)

# WebView2: prefer NuGet/CMake package, fallback to environment variable.
find_package(WebView2 CONFIG QUIET)
if (WebView2_FOUND)
    target_link_libraries(LocalJarvis PRIVATE WebView2::WebView2)
else()
    if (DEFINED ENV{WEBVIEW2_SDK_PATH})
        target_include_directories(LocalJarvis PRIVATE "$ENV{WEBVIEW2_SDK_PATH}/build/native/include")
        target_link_directories(LocalJarvis PRIVATE "$ENV{WEBVIEW2_SDK_PATH}/build/native/x64")
        target_link_libraries(LocalJarvis PRIVATE WebView2LoaderStatic)
    else()
        message(FATAL_ERROR "WebView2 SDK not found. Install Microsoft.Web.WebView2 or set WEBVIEW2_SDK_PATH.")
    endif()
endif()
