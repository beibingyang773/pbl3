#include "Input.h"

#include <conio.h>

InputEvent Input::Poll() const {
    InputEvent event;
    if (!_kbhit()) {
        return event;
    }

    int key = _getch();

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
        default:
            break;
    }

    return event;
}
