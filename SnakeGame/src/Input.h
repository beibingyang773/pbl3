#pragma once

#include <optional>
#include <vector>

#include "gameplay/Snake.h"

enum class InputAction {
    None = 0,
    Quit,
    TogglePause,
    Save,
    Load,
    StartOrRestart,
    SetDirectInput,
    SetInputQueue,
    SetLatestOverride,
    ToggleStressTest
};

struct InputEvent {
    std::optional<Direction> direction;
    InputAction action = InputAction::None;
};

class Input {
public:
    std::vector<InputEvent> PollAll() const;

    static bool IsOpposite(Direction a, Direction b);
};
