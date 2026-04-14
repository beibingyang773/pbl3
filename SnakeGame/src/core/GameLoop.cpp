#include "core/GameLoop.h"

void GameLoop::Run(
    const std::function<bool()>& shouldContinue,
    const std::function<void()>& processInput,
    const std::function<void()>& update,
    const std::function<void()>& render) const {
    while (shouldContinue()) {
        processInput();
        update();
        render();
    }
}
