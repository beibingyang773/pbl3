#pragma once

#include <string>

#include "Renderer.h"
#include "systems/InputSystem.h"

class RenderSystem {
public:
    RenderSystem(int width, int height);

    void Draw(
        const Snake& snake,
        const Point& food,
        int score,
        int speedLevel,
        const std::string& speedName,
        InputExperimentMode inputMode,
        const InputRuntimeStats& inputStats,
        bool stressTestEnabled,
        const std::string& statusMessage,
        bool menu,
        bool paused,
        bool gameOver) const;

private:
    Renderer renderer_;
};
