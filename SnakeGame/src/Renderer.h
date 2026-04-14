#pragma once

#include <string>
#include <vector>

#include "gameplay/Snake.h"
#include "systems/InputSystem.h"

class Renderer {
public:
    Renderer(int width, int height);

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
    int width_;
    int height_;
    void DrawCell(std::vector<char>& buffer, int x, int y, char c) const;
};
