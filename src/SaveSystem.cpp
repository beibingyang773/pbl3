#include "SaveSystem.h"

#include <fstream>
#include <sstream>

namespace {

std::vector<std::string> Split(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            tokens.push_back(item);
        }
    }
    return tokens;
}

bool ParsePoint(const std::string& token, Point& p) {
    const auto parts = Split(token, ',');
    if (parts.size() != 2) {
        return false;
    }

    try {
        p.x = std::stoi(parts[0]);
        p.y = std::stoi(parts[1]);
    } catch (...) {
        return false;
    }
    return true;
}

}  // namespace

bool SaveSystem::SaveToText(const std::string& filePath, const GameState& state) const {
    std::ofstream out(filePath, std::ios::trunc);
    if (!out.is_open()) {
        return false;
    }

    out << "score=" << state.score << '\n';
    out << "dir=" << state.direction << '\n';

    out << "snake=";
    for (size_t i = 0; i < state.snake.size(); ++i) {
        out << state.snake[i].x << ',' << state.snake[i].y;
        if (i + 1 < state.snake.size()) {
            out << ';';
        }
    }
    out << '\n';

    out << "food=" << state.food.x << ',' << state.food.y << '\n';
    return true;
}

bool SaveSystem::LoadFromText(const std::string& filePath, GameState& state) const {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return false;
    }

    GameState loaded;
    bool scoreFound = false;
    bool dirFound = false;
    bool snakeFound = false;
    bool foodFound = false;

    std::string line;
    while (std::getline(in, line)) {
        const auto eqPos = line.find('=');
        if (eqPos == std::string::npos) {
            continue;
        }

        const std::string key = line.substr(0, eqPos);
        const std::string value = line.substr(eqPos + 1);

        try {
            if (key == "score") {
                loaded.score = std::stoi(value);
                scoreFound = true;
            } else if (key == "dir") {
                loaded.direction = std::stoi(value);
                dirFound = true;
            } else if (key == "snake") {
                const auto points = Split(value, ';');
                loaded.snake.clear();
                for (const auto& token : points) {
                    Point p;
                    if (!ParsePoint(token, p)) {
                        return false;
                    }
                    loaded.snake.push_back(p);
                }
                snakeFound = !loaded.snake.empty();
            } else if (key == "food") {
                Point p;
                if (!ParsePoint(value, p)) {
                    return false;
                }
                loaded.food = p;
                foodFound = true;
            }
        } catch (...) {
            return false;
        }
    }

    if (!scoreFound || !dirFound || !snakeFound || !foodFound) {
        return false;
    }

    state = loaded;
    return true;
}
