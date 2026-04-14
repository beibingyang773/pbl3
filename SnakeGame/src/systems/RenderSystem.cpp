#include "systems/RenderSystem.h"

RenderSystem::RenderSystem(int width, int height) : renderer_(width, height) {}

void RenderSystem::Draw(
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
    bool gameOver) const {
    renderer_.Draw(
        snake,
        food,
        score,
        speedLevel,
        speedName,
        inputMode,
        inputStats,
        stressTestEnabled,
        statusMessage,
        menu,
        paused,
        gameOver);
}
