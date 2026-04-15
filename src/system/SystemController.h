#pragma once

#include "includes.h"

namespace jarvis {

class SystemController {
public:
    bool ExecuteAction(const std::string& action, std::string& resultMessage);
};

} // namespace jarvis
