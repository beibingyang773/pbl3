#include "Game.h"

#include <chrono>
#include <thread>

Game::Game(int width, int height, std::string saveFile)
    : width_(width),
      height_(height),
      saveFile_(std::move(saveFile)),
      snake_(width / 2, height / 2),
      food_(width, height),
      renderSystem_(width, height) {
    food_.Respawn(snake_);
}

void Game::Run() {
    gameLoop_.Run(
        [this]() { return stateMachine_.GetState() != GameStatus::Exit; },
        [this]() { ProcessInput(); },
        [this]() { TickUpdate(); },
        [this]() { Render(); });
}

void Game::ProcessInput() {
    const FrameInput frameInput = inputSystem_.Collect(snake_.GetDirection());

    if (frameInput.requestQuit) {
        stateMachine_.RequestExit();
        return;
    }

    if (frameInput.switchedMode.has_value()) {
        if (*frameInput.switchedMode == InputExperimentMode::DirectInput) {
            SetStatusMessage("Input mode switched: Direct", 150);
        } else if (*frameInput.switchedMode == InputExperimentMode::InputQueue) {
            SetStatusMessage("Input mode switched: Queue", 150);
        } else {
            SetStatusMessage("Input mode switched: Latest", 150);
        }
    }

    if (frameInput.requestToggleStressTest) {
        const bool enabled = inputSystem_.ToggleStressTest();
        SetStatusMessage(enabled ? "Stress test enabled" : "Stress test disabled", 150);
    }

    if (frameInput.requestStartOrRestart) {
        if (stateMachine_.GetState() == GameStatus::Menu || stateMachine_.GetState() == GameStatus::GameOver) {
            Reset();
            stateMachine_.StartFromMenuOrRestart();
        }
    }

    if (frameInput.requestTogglePause) {
        stateMachine_.TogglePause();
    }

    if (frameInput.requestSave && stateMachine_.GetState() != GameStatus::Menu) {
        const auto state = BuildState();
        if (saveSystem_.SaveToText(saveFile_, state)) {
            SetStatusMessage("Save success", 120);
        } else {
            SetStatusMessage("Save failed", 180);
        }
    }

    if (frameInput.requestLoad) {
        GameState state;
        if (saveSystem_.LoadFromText(saveFile_, state)) {
            if (ApplyState(state)) {
                stateMachine_.OnLoadSuccess();
                SetStatusMessage("Load success", 120);
            } else {
                SetStatusMessage("Load failed: invalid state", 180);
            }
        } else {
            SetStatusMessage("Load failed: file damaged", 180);
        }
    }

    if (frameInput.direction.has_value() && stateMachine_.CanAcceptDirection()) {
        snake_.SetDirection(*frameInput.direction);
    }
}

void Game::TickUpdate() {
    const int ticks = timeSystem_.Step(stateMachine_.CanUpdate());
    for (int i = 0; i < ticks; ++i) {
        Update();
        if (!stateMachine_.CanUpdate()) {
            break;
        }
    }

    if (statusMessageFrames_ > 0) {
        --statusMessageFrames_;
        if (statusMessageFrames_ == 0) {
            statusMessage_.clear();
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Game::Update() {
    Point nextHead = snake_.GetHead();
    switch (snake_.GetDirection()) {
        case Direction::Up:
            --nextHead.y;
            break;
        case Direction::Right:
            ++nextHead.x;
            break;
        case Direction::Down:
            ++nextHead.y;
            break;
        case Direction::Left:
            --nextHead.x;
            break;
    }

    const bool willGrow = (nextHead == food_.GetPosition());
    snake_.Move(willGrow);

    if (willGrow) {
        score_ += 10;
        food_.Respawn(snake_);
        timeSystem_.UpdateSpeedByScore(score_);
    }

    if (HitsWall(snake_.GetHead()) || snake_.HitsSelf()) {
        stateMachine_.OnSnakeDead();
    }
}

void Game::Render() {
    const GameStatus state = stateMachine_.GetState();
    renderSystem_.Draw(
        snake_,
        food_.GetPosition(),
        score_,
        timeSystem_.GetSpeedLevelNumber(),
        timeSystem_.GetSpeedLevelName(),
        inputSystem_.GetMode(),
        inputSystem_.GetStats(),
        inputSystem_.IsStressTestEnabled(),
        statusMessage_,
        state == GameStatus::Menu,
        state == GameStatus::Paused,
        state == GameStatus::GameOver);
}

bool Game::HitsWall(const Point& p) const {
    return p.x <= 0 || p.x >= width_ + 1 || p.y <= 0 || p.y >= height_ + 1;
}

void Game::Reset() {
    inputSystem_.ResetStats();
    snake_ = Snake(width_ / 2, height_ / 2);
    food_.Respawn(snake_);
    score_ = 0;
    timeSystem_.UpdateSpeedByScore(score_);
    SetStatusMessage("Game started", 90);
}

void Game::SetStatusMessage(const std::string& message, int showFrames) {
    statusMessage_ = message;
    statusMessageFrames_ = showFrames;
}

GameState Game::BuildState() const {
    GameState state;
    state.snake = snake_.ToVector();
    state.food = food_.GetPosition();
    state.direction = static_cast<int>(snake_.GetDirection());
    state.score = score_;
    return state;
}

bool Game::ApplyState(const GameState& state) {
    if (state.snake.size() < 2) {
        return false;
    }
    if (state.food.x <= 0 || state.food.x >= width_ + 1 || state.food.y <= 0 || state.food.y >= height_ + 1) {
        return false;
    }
    if (state.direction < 0 || state.direction > 3) {
        return false;
    }

    for (const auto& p : state.snake) {
        if (p.x <= 0 || p.x >= width_ + 1 || p.y <= 0 || p.y >= height_ + 1) {
            return false;
        }
    }

    snake_.SetBodyFromVector(state.snake);
    snake_.SetDirection(static_cast<Direction>(state.direction));
    food_.SetPosition(state.food);
    score_ = state.score;

    if (snake_.HitsSelf() || snake_.Occupies(food_.GetPosition())) {
        return false;
    }

    return true;
}
