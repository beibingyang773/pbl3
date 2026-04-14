#include "systems/TimeSystem.h"

#include <algorithm>

TimeSystem::TimeSystem()
    : lastTick_(Clock::now()), tickInterval_(std::chrono::milliseconds(160)) {}

int TimeSystem::Step(bool allowUpdate) {
    const auto now = Clock::now();
    auto delta = now - lastTick_;
    lastTick_ = now;

    const auto maxDelta = std::chrono::milliseconds(250);
    if (delta > maxDelta) {
        delta = maxDelta;
    }

    if (!allowUpdate) {
        accumulator_ = Clock::duration::zero();
        return 0;
    }

    accumulator_ += delta;

    int ticks = 0;
    while (accumulator_ >= tickInterval_) {
        accumulator_ -= tickInterval_;
        ++ticks;
    }

    return ticks;
}

void TimeSystem::UpdateSpeedByScore(int score) {
    if (score >= 120) {
        speedLevel_ = SpeedLevel::High;
        tickInterval_ = std::chrono::milliseconds(90);
    } else if (score >= 50) {
        speedLevel_ = SpeedLevel::Medium;
        tickInterval_ = std::chrono::milliseconds(120);
    } else {
        speedLevel_ = SpeedLevel::Low;
        tickInterval_ = std::chrono::milliseconds(160);
    }
}

int TimeSystem::GetSpeedLevelNumber() const {
    return static_cast<int>(speedLevel_) + 1;
}

const char* TimeSystem::GetSpeedLevelName() const {
    switch (speedLevel_) {
        case SpeedLevel::Low:
            return "Low";
        case SpeedLevel::Medium:
            return "Medium";
        case SpeedLevel::High:
            return "High";
        default:
            return "Low";
    }
}
