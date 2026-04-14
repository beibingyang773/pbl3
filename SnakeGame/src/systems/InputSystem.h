#pragma once

#include <deque>
#include <optional>

#include "Input.h"

enum class InputExperimentMode {
    DirectInput = 0,
    InputQueue,
    LatestLegalOverride
};

struct InputRuntimeStats {
    int directionReadCount = 0;
    int directionAcceptedCount = 0;
    int directionRejectedCount = 0;
    int queueEnqueuedCount = 0;
    int queueDroppedCount = 0;
    int queueExecutedCount = 0;
    int stressInjectedCount = 0;
    int modeSwitchCount = 0;
    int lastAppliedDelayFrames = 0;
    int queueCurrentSize = 0;
    int queueAverageSize = 0;
};

struct FrameInput {
    std::optional<Direction> direction;
    std::optional<InputExperimentMode> switchedMode;
    bool requestQuit = false;
    bool requestTogglePause = false;
    bool requestSave = false;
    bool requestLoad = false;
    bool requestStartOrRestart = false;
    bool requestToggleStressTest = false;
};

class InputSystem {
public:
    FrameInput Collect(Direction currentDirection);
    InputExperimentMode GetMode() const;
    const InputRuntimeStats& GetStats() const;
    bool IsStressTestEnabled() const;
    bool ToggleStressTest();
    void ResetStats();

private:
    struct QueuedDirection {
        Direction direction;
        int ageFrames = 0;
    };

    std::vector<Direction> BuildStressDirections(Direction currentDirection);
    bool TryAcceptDirection(Direction inputDirection, Direction baseDirection) const;

    Input input_;
    InputExperimentMode mode_ = InputExperimentMode::LatestLegalOverride;
    std::deque<QueuedDirection> directionQueue_;
    InputRuntimeStats stats_;
    bool stressTestEnabled_ = false;
    int stressSeed_ = 0;
    int queueSampleSum_ = 0;
    int queueSampleCount_ = 0;
};
