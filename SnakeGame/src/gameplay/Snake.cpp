#include "gameplay/Snake.h"

#include <stdexcept>

Snake::Snake(int startX, int startY) {
    body_.push_front({startX, startY});
    body_.push_back({startX - 1, startY});
    body_.push_back({startX - 2, startY});
}

void Snake::SetDirection(Direction newDirection) {
    if (!IsOpposite(direction_, newDirection)) {
        direction_ = newDirection;
    }
}

Direction Snake::GetDirection() const {
    return direction_;
}

Point Snake::GetHead() const {
    return body_.front();
}

const std::deque<Point>& Snake::GetBody() const {
    return body_;
}

void Snake::Move(bool grow) {
    Point next = GetHead();
    switch (direction_) {
        case Direction::Up:
            --next.y;
            break;
        case Direction::Right:
            ++next.x;
            break;
        case Direction::Down:
            ++next.y;
            break;
        case Direction::Left:
            --next.x;
            break;
    }

    body_.push_front(next);
    if (!grow) {
        body_.pop_back();
    }
}

bool Snake::HitsSelf() const {
    const Point head = GetHead();
    for (size_t i = 1; i < body_.size(); ++i) {
        if (body_[i] == head) {
            return true;
        }
    }
    return false;
}

bool Snake::Occupies(const Point& p) const {
    for (const auto& segment : body_) {
        if (segment == p) {
            return true;
        }
    }
    return false;
}

void Snake::SetBodyFromVector(const std::vector<Point>& body) {
    if (body.size() < 2) {
        throw std::runtime_error("Invalid snake body in save data");
    }

    body_.clear();
    for (const auto& p : body) {
        body_.push_back(p);
    }
}

std::vector<Point> Snake::ToVector() const {
    return std::vector<Point>(body_.begin(), body_.end());
}

bool Snake::IsOpposite(Direction a, Direction b) const {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}
