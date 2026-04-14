#pragma once

enum class GameStatus {
    Menu = 0,
    Running,
    Paused,
    GameOver,
    Exit
};

class StateMachine {
public:
    GameStatus GetState() const;

    bool CanUpdate() const;
    bool CanAcceptDirection() const;

    void RequestExit();
    void StartFromMenuOrRestart();
    void TogglePause();
    void OnSnakeDead();
    void OnLoadSuccess();

private:
    GameStatus state_ = GameStatus::Menu;
};
