#pragma once

#include <string>

#include "core/GameLoop.h"
#include "core/StateMachine.h"
#include "gameplay/Food.h"
#include "gameplay/Snake.h"
#include "persistence/SaveSystem.h"
#include "systems/InputSystem.h"
#include "systems/RenderSystem.h"
#include "systems/TimeSystem.h"

class Game {
public:
    Game(int width, int height, std::string saveFile);
    void Run();

private:
    int width_;
    int height_;
    std::string saveFile_;

    GameLoop gameLoop_;
    Snake snake_;
    Food food_;
    RenderSystem renderSystem_;
    InputSystem inputSystem_;
    TimeSystem timeSystem_;
    StateMachine stateMachine_;
    SaveSystem saveSystem_;

    int score_ = 0;
    std::string statusMessage_;
    int statusMessageFrames_ = 0;

    void ProcessInput();
    void Update();
    void Render();
    void TickUpdate();

    bool HitsWall(const Point& p) const;
    void Reset();
    void SetStatusMessage(const std::string& message, int showFrames);

    GameState BuildState() const;
    bool ApplyState(const GameState& state);
};
