#pragma once

#include <chrono>

class TimeSystem {
public:
    enum class SpeedLevel {
        Low = 0,
        Medium,
        High
    };

    TimeSystem();

    int Step(bool allowUpdate);
    void UpdateSpeedByScore(int score);

    int GetSpeedLevelNumber() const;
    const char* GetSpeedLevelName() const;

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point lastTick_;
    Clock::duration accumulator_{};
    std::chrono::milliseconds tickInterval_;
    SpeedLevel speedLevel_ = SpeedLevel::Low;
};
