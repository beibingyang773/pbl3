#include "Game.h"

#include <chrono>
#include <thread>

Game::Game(int width, int height, std::string saveFile)
    : width_(width),
      height_(height),
      saveFile_(std::move(saveFile)),
      snake_(width / 2, height / 2),
      food_(width, height),
      renderer_(width, height) {
    food_.Respawn(snake_);
}

void Game::Run() {
    status_ = Status::Running;

    constexpr auto frameTime = std::chrono::milliseconds(120);

    while (status_ != Status::Quit) {
        const auto frameStart = std::chrono::steady_clock::now();

        ProcessInput();
        if (status_ == Status::Running) {
            Update();
        }
        Render();

        const auto elapsed = std::chrono::steady_clock::now() - frameStart;
        if (elapsed < frameTime) {
            std::this_thread::sleep_for(frameTime - elapsed);
        }
    }
}

void Game::ProcessInput() {
    const InputEvent event = input_.Poll();

    if (event.action == InputAction::Quit) {
        status_ = Status::Quit;
        return;
    }

    if (event.action == InputAction::TogglePause) {
        if (status_ == Status::Running) {
            status_ = Status::Paused;
        } else if (status_ == Status::Paused) {
            status_ = Status::Running;
        }
    }

    if (event.action == InputAction::Save) {
        const auto state = BuildState();
        saveSystem_.SaveToText(saveFile_, state);
    }

    if (event.action == InputAction::Load) {
        GameState state;
        if (saveSystem_.LoadFromText(saveFile_, state)) {
            if (ApplyState(state)) {
                status_ = Status::Running;
            }
        }
    }

    if (event.direction.has_value() && status_ == Status::Running) {
        snake_.SetDirection(*event.direction);
    }
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
    }

    if (HitsWall(snake_.GetHead()) || snake_.HitsSelf()) {
        status_ = Status::GameOver;
    }
}

void Game::Render() {
    const bool paused = (status_ == Status::Paused);
    const bool gameOver = (status_ == Status::GameOver);
    renderer_.Draw(snake_, food_.GetPosition(), score_, paused, gameOver);
}

bool Game::HitsWall(const Point& p) const {
    return p.x <= 0 || p.x >= width_ + 1 || p.y <= 0 || p.y >= height_ + 1;
}

void Game::Reset() {
    snake_ = Snake(width_ / 2, height_ / 2);
    food_.Respawn(snake_);
    score_ = 0;
    status_ = Status::Running;
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
