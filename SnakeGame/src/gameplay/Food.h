#pragma once

#include <random>

#include "gameplay/Snake.h"

class Food {
public:
    Food(int width, int height);

    const Point& GetPosition() const;
    void Respawn(const Snake& snake);
    void SetPosition(const Point& p);

private:
    int width_;
    int height_;
    Point position_;

    std::mt19937 rng_;
};
