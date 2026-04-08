#include "Renderer.h"

#include <windows.h>

#include <algorithm>
#include <string>

Renderer::Renderer(int width, int height) : width_(width), height_(height) {
    constexpr int hudWidth = 28;
    const int frameW = width_ + 2 + hudWidth;
    const int frameH = height_ + 4;

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo{};
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(out, &cursorInfo);

    // Keep screen buffer size consistent with our frame size.
    COORD bufferSize;
    bufferSize.X = static_cast<SHORT>(frameW);
    bufferSize.Y = static_cast<SHORT>(frameH);
    SetConsoleScreenBufferSize(out, bufferSize);

    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = static_cast<SHORT>(frameW - 1);
    windowSize.Bottom = static_cast<SHORT>(frameH - 1);
    SetConsoleWindowInfo(out, TRUE, &windowSize);
}

void Renderer::Draw(
    const Snake& snake,
    const Point& food,
    int score,
    int difficultyLevel,
    const std::string& difficultyName,
    bool paused,
    bool gameOver) const {
    constexpr int hudWidth = 28;
    constexpr int hudGap = 2;
    const int frameW = width_ + 2 + hudWidth;
    const int frameH = height_ + 4;
    const int gameAreaW = width_ + 2;
    std::vector<char> buffer(static_cast<size_t>(frameW * frameH), ' ');
    const int hudStartX = gameAreaW + hudGap;

    for (int x = 0; x < gameAreaW; ++x) {
        DrawCell(buffer, x, 1, '#');
        DrawCell(buffer, x, height_ + 2, '#');
    }

    for (int y = 1; y <= height_ + 2; ++y) {
        DrawCell(buffer, 0, y, '#');
        DrawCell(buffer, width_ + 1, y, '#');
        DrawCell(buffer, gameAreaW, y, '|');
    }

    const std::string hudTitle = "Game Info";
    for (size_t i = 0; i < hudTitle.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 0, hudTitle[i]);
    }

    const std::string scoreLine = "Score: " + std::to_string(score);
    for (size_t i = 0; i < scoreLine.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 2, scoreLine[i]);
    }

    const std::string difficultyLine = "Lv" + std::to_string(difficultyLevel) + "-" + difficultyName;
    for (size_t i = 0; i < difficultyLine.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 3, difficultyLine[i]);
    }

    const std::string pauseLine = "[P] Pause";
    for (size_t i = 0; i < pauseLine.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 5, pauseLine[i]);
    }

    const std::string saveLine = "[K] Save";
    for (size_t i = 0; i < saveLine.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 6, saveLine[i]);
    }

    const std::string loadLine = "[L] Load";
    for (size_t i = 0; i < loadLine.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 7, loadLine[i]);
    }

    const std::string quitLine = "[Q] Quit";
    for (size_t i = 0; i < quitLine.size() && hudStartX + static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, hudStartX + static_cast<int>(i), 8, quitLine[i]);
    }

    const auto& body = snake.GetBody();
    for (size_t i = 0; i < body.size(); ++i) {
        const char bodyChar = (i == 0) ? '@' : 'o';
        DrawCell(buffer, body[i].x, body[i].y + 1, bodyChar);
    }

    DrawCell(buffer, food.x, food.y + 1, '*');

    if (paused) {
        const std::string msg = "PAUSED";
        const int startX = std::max(1, (gameAreaW - static_cast<int>(msg.size())) / 2);
        const int y = height_ / 2 + 1;
        for (size_t i = 0; i < msg.size(); ++i) {
            DrawCell(buffer, startX + static_cast<int>(i), y, msg[i]);
        }
    }

    if (gameOver) {
        const std::string msg = "GAME OVER - Press L to Load or Q to Quit";
        const int startX = std::max(1, (gameAreaW - static_cast<int>(msg.size())) / 2);
        const int y = height_ / 2 + 2;
        for (size_t i = 0; i < msg.size() && startX + static_cast<int>(i) < gameAreaW - 1; ++i) {
            DrawCell(buffer, startX + static_cast<int>(i), y, msg[i]);
        }
    }

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD origin{0, 0};
    DWORD written = 0;

    SetConsoleCursorPosition(out, origin);
    WriteConsoleOutputCharacterA(
        out,
        buffer.data(),
        static_cast<DWORD>(buffer.size()),
        origin,
        &written);
}

void Renderer::DrawCell(std::vector<char>& buffer, int x, int y, char c) const {
    constexpr int hudWidth = 28;
    const int frameW = width_ + 2 + hudWidth;
    const int frameH = height_ + 4;
    if (x < 0 || x >= frameW || y < 0 || y >= frameH) {
        return;
    }

    buffer[static_cast<size_t>(y * frameW + x)] = c;
}
