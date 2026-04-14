#include "Input.h"

#include <conio.h>

namespace {

InputEvent ParseSingleEvent(int key) {
    InputEvent event;

    // Arrow keys use a two-byte sequence in Windows console.
    if (key == 0 || key == 224) {
        const int arrow = _getch();
        switch (arrow) {
            case 72:
                event.direction = Direction::Up;
                return event;
            case 77:
                event.direction = Direction::Right;
                return event;
            case 80:
                event.direction = Direction::Down;
                return event;
            case 75:
                event.direction = Direction::Left;
                return event;
            default:
                return event;
        }
    }

    switch (key) {
        case 'w':
        case 'W':
            event.direction = Direction::Up;
            break;
        case 'd':
        case 'D':
            event.direction = Direction::Right;
            break;
        case 's':
        case 'S':
            event.direction = Direction::Down;
            break;
        case 'a':
        case 'A':
            event.direction = Direction::Left;
            break;
        case 'q':
        case 'Q':
            event.action = InputAction::Quit;
            break;
        case 'p':
        case 'P':
            event.action = InputAction::TogglePause;
            break;
        case 'k':
        case 'K':
            event.action = InputAction::Save;
            break;
        case 'l':
        case 'L':
            event.action = InputAction::Load;
            break;
        case 'r':
        case 'R':
        case ' ':
        case 13:
            event.action = InputAction::StartOrRestart;
            break;
        case '1':
            event.action = InputAction::SetDirectInput;
            break;
        case '2':
            event.action = InputAction::SetInputQueue;
            break;
        case '3':
            event.action = InputAction::SetLatestOverride;
            break;
        case 't':
        case 'T':
            event.action = InputAction::ToggleStressTest;
            break;
        default:
            break;
    }

    return event;
}

}  // namespace

std::vector<InputEvent> Input::PollAll() const {
    std::vector<InputEvent> events;
    while (_kbhit()) {
        const int key = _getch();
        events.push_back(ParseSingleEvent(key));
    }
    return events;
}

bool Input::IsOpposite(Direction a, Direction b) {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}
