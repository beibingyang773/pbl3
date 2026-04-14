#include "core/StateMachine.h"

GameStatus StateMachine::GetState() const {
    return state_;
}

bool StateMachine::CanUpdate() const {
    return state_ == GameStatus::Running;
}

bool StateMachine::CanAcceptDirection() const {
    return state_ == GameStatus::Running;
}

void StateMachine::RequestExit() {
    state_ = GameStatus::Exit;
}

void StateMachine::StartFromMenuOrRestart() {
    if (state_ == GameStatus::Menu || state_ == GameStatus::GameOver) {
        state_ = GameStatus::Running;
    }
}

void StateMachine::TogglePause() {
    if (state_ == GameStatus::Running) {
        state_ = GameStatus::Paused;
    } else if (state_ == GameStatus::Paused) {
        state_ = GameStatus::Running;
    }
}

void StateMachine::OnSnakeDead() {
    if (state_ == GameStatus::Running) {
        state_ = GameStatus::GameOver;
    }
}

void StateMachine::OnLoadSuccess() {
    if (state_ != GameStatus::Exit) {
        state_ = GameStatus::Running;
    }
}
