#pragma once

#include <string>
#include <vector>

#include "gameplay/Snake.h"

struct GameState {
    std::vector<Point> snake;
    Point food;
    int direction = static_cast<int>(Direction::Right);
    int score = 0;
};

class SaveSystem {
public:
    bool SaveToText(const std::string& filePath, const GameState& state) const;
    bool LoadFromText(const std::string& filePath, GameState& state) const;
};
