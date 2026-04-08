#pragma once

#include <optional>

#include "Snake.h"

enum class InputAction {
    None = 0,
    Quit,
    TogglePause,
    Save,
    Load
};

struct InputEvent {
    std::optional<Direction> direction;
    InputAction action = InputAction::None;
};

class Input {
public:
    InputEvent Poll() const;
};
