#pragma once

#include <deque>
#include <vector>

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum class Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

class Snake {
public:
    Snake(int startX, int startY);

    void SetDirection(Direction newDirection);
    Direction GetDirection() const;

    Point GetHead() const;
    const std::deque<Point>& GetBody() const;

    void Move(bool grow);
    bool HitsSelf() const;
    bool Occupies(const Point& p) const;

    void SetBodyFromVector(const std::vector<Point>& body);
    std::vector<Point> ToVector() const;

private:
    std::deque<Point> body_;
    Direction direction_ = Direction::Right;

    bool IsOpposite(Direction a, Direction b) const;
};
