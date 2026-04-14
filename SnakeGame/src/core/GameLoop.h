#pragma once

#include <functional>

class GameLoop {
public:
    void Run(
        const std::function<bool()>& shouldContinue,
        const std::function<void()>& processInput,
        const std::function<void()>& update,
        const std::function<void()>& render) const;
};
