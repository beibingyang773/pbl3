#pragma once

#include <string>
#include <vector>

#include "Snake.h"

class Renderer {
public:
    Renderer(int width, int height);

    void Draw(
        const Snake& snake,
        const Point& food,
        int score,
        int difficultyLevel,
        const std::string& difficultyName,
        bool paused,
        bool gameOver) const;

private:
    int width_;
    int height_;
    void DrawCell(std::vector<char>& buffer, int x, int y, char c) const;
};
