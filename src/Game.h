#pragma once

#include <string>

#include "Food.h"
#include "Input.h"
#include "Renderer.h"
#include "SaveSystem.h"
#include "Snake.h"

class Game {
public:
    Game(int width, int height, std::string saveFile);
    void Run();

private:
    enum class Status {
        Start,
        Running,
        Paused,
        GameOver,
        Quit
    };

    int width_;
    int height_;
    std::string saveFile_;

    Snake snake_;
    Food food_;
    Renderer renderer_;
    Input input_;
    SaveSystem saveSystem_;

    int score_ = 0;
    Status status_ = Status::Start;

    void ProcessInput();
    void Update();
    void Render();

    bool HitsWall(const Point& p) const;
    void Reset();
    int GetDifficultyLevel() const;
    std::string GetDifficultyName() const;
    int GetFrameDelayMs() const;

    GameState BuildState() const;
    bool ApplyState(const GameState& state);
};
