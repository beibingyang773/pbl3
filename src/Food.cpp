#include "Food.h"

#include <chrono>

Food::Food(int width, int height)
    : width_(width), height_(height), position_{width / 2, height / 2} {
    const auto seed = static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count());
    rng_.seed(seed);
}

const Point& Food::GetPosition() const {
    return position_;
}

void Food::Respawn(const Snake& snake) {
    std::uniform_int_distribution<int> xDist(1, width_ - 2);
    std::uniform_int_distribution<int> yDist(1, height_ - 2);

    Point candidate;
    do {
        candidate = {xDist(rng_), yDist(rng_)};
    } while (snake.Occupies(candidate));

    position_ = candidate;
}

void Food::SetPosition(const Point& p) {
    position_ = p;
}
