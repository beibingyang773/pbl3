#include "systems/InputSystem.h"

#include <array>

FrameInput InputSystem::Collect(Direction currentDirection) {
    FrameInput frame;
    std::optional<Direction> firstDirection;
    Direction latestDirection = currentDirection;

    for (auto& item : directionQueue_) {
        ++item.ageFrames;
    }

    const auto events = input_.PollAll();
    for (const auto& event : events) {
        if (event.action == InputAction::Quit) {
            frame.requestQuit = true;
        } else if (event.action == InputAction::TogglePause) {
            frame.requestTogglePause = true;
        } else if (event.action == InputAction::Save) {
            frame.requestSave = true;
        } else if (event.action == InputAction::Load) {
            frame.requestLoad = true;
        } else if (event.action == InputAction::StartOrRestart) {
            frame.requestStartOrRestart = true;
        } else if (event.action == InputAction::ToggleStressTest) {
            frame.requestToggleStressTest = true;
        } else if (event.action == InputAction::SetDirectInput) {
            mode_ = InputExperimentMode::DirectInput;
            directionQueue_.clear();
            frame.switchedMode = mode_;
            ++stats_.modeSwitchCount;
        } else if (event.action == InputAction::SetInputQueue) {
            mode_ = InputExperimentMode::InputQueue;
            directionQueue_.clear();
            frame.switchedMode = mode_;
            ++stats_.modeSwitchCount;
        } else if (event.action == InputAction::SetLatestOverride) {
            mode_ = InputExperimentMode::LatestLegalOverride;
            directionQueue_.clear();
            frame.switchedMode = mode_;
            ++stats_.modeSwitchCount;
        }

        if (!event.direction.has_value()) {
            continue;
        }

        const Direction inputDirection = *event.direction;
        ++stats_.directionReadCount;

        if (mode_ == InputExperimentMode::DirectInput) {
            if (!firstDirection.has_value() && TryAcceptDirection(inputDirection, currentDirection)) {
                firstDirection = inputDirection;
                ++stats_.directionAcceptedCount;
            } else if (!TryAcceptDirection(inputDirection, currentDirection)) {
                ++stats_.directionRejectedCount;
            }
        } else if (mode_ == InputExperimentMode::InputQueue) {
            const Direction baseDirection = directionQueue_.empty() ? currentDirection : directionQueue_.back().direction;
            if (TryAcceptDirection(inputDirection, baseDirection) && inputDirection != baseDirection) {
                constexpr size_t maxQueueSize = 8;
                if (directionQueue_.size() < maxQueueSize) {
                    directionQueue_.push_back({inputDirection, 0});
                    ++stats_.queueEnqueuedCount;
                    ++stats_.directionAcceptedCount;
                } else {
                    ++stats_.queueDroppedCount;
                }
            } else if (!TryAcceptDirection(inputDirection, baseDirection)) {
                ++stats_.directionRejectedCount;
            }
        } else {
            if (TryAcceptDirection(inputDirection, currentDirection)) {
                latestDirection = inputDirection;
                ++stats_.directionAcceptedCount;
            } else {
                ++stats_.directionRejectedCount;
            }
        }
    }

    if (stressTestEnabled_) {
        const auto stressDirections = BuildStressDirections(currentDirection);
        stats_.stressInjectedCount += static_cast<int>(stressDirections.size());

        for (Direction inputDirection : stressDirections) {
            ++stats_.directionReadCount;

            if (mode_ == InputExperimentMode::DirectInput) {
                if (!firstDirection.has_value() && TryAcceptDirection(inputDirection, currentDirection)) {
                    firstDirection = inputDirection;
                    ++stats_.directionAcceptedCount;
                } else if (!TryAcceptDirection(inputDirection, currentDirection)) {
                    ++stats_.directionRejectedCount;
                }
            } else if (mode_ == InputExperimentMode::InputQueue) {
                const Direction baseDirection = directionQueue_.empty() ? currentDirection : directionQueue_.back().direction;
                if (TryAcceptDirection(inputDirection, baseDirection) && inputDirection != baseDirection) {
                    constexpr size_t maxQueueSize = 8;
                    if (directionQueue_.size() < maxQueueSize) {
                        directionQueue_.push_back({inputDirection, 0});
                        ++stats_.queueEnqueuedCount;
                        ++stats_.directionAcceptedCount;
                    } else {
                        ++stats_.queueDroppedCount;
                    }
                } else if (!TryAcceptDirection(inputDirection, baseDirection)) {
                    ++stats_.directionRejectedCount;
                }
            } else {
                if (TryAcceptDirection(inputDirection, currentDirection)) {
                    latestDirection = inputDirection;
                    ++stats_.directionAcceptedCount;
                } else {
                    ++stats_.directionRejectedCount;
                }
            }
        }
    }

    if (mode_ == InputExperimentMode::DirectInput) {
        frame.direction = firstDirection;
        if (frame.direction.has_value()) {
            stats_.lastAppliedDelayFrames = 0;
        }
    } else if (mode_ == InputExperimentMode::InputQueue) {
        if (!directionQueue_.empty()) {
            frame.direction = directionQueue_.front().direction;
            stats_.lastAppliedDelayFrames = directionQueue_.front().ageFrames;
            ++stats_.queueExecutedCount;
            directionQueue_.pop_front();
        }
    } else if (latestDirection != currentDirection) {
        frame.direction = latestDirection;
        stats_.lastAppliedDelayFrames = 0;
    }

    stats_.queueCurrentSize = static_cast<int>(directionQueue_.size());
    queueSampleSum_ += stats_.queueCurrentSize;
    ++queueSampleCount_;
    stats_.queueAverageSize = queueSampleCount_ == 0 ? 0 : queueSampleSum_ / queueSampleCount_;

    return frame;
}

InputExperimentMode InputSystem::GetMode() const {
    return mode_;
}

bool InputSystem::TryAcceptDirection(Direction inputDirection, Direction baseDirection) const {
    return !Input::IsOpposite(inputDirection, baseDirection);
}

const InputRuntimeStats& InputSystem::GetStats() const {
    return stats_;
}

bool InputSystem::IsStressTestEnabled() const {
    return stressTestEnabled_;
}

bool InputSystem::ToggleStressTest() {
    stressTestEnabled_ = !stressTestEnabled_;
    return stressTestEnabled_;
}

void InputSystem::ResetStats() {
    stats_ = InputRuntimeStats{};
    queueSampleSum_ = 0;
    queueSampleCount_ = 0;
    directionQueue_.clear();
}

std::vector<Direction> InputSystem::BuildStressDirections(Direction currentDirection) {
    static constexpr std::array<Direction, 4> pattern = {
        Direction::Up,
        Direction::Left,
        Direction::Down,
        Direction::Right};

    std::vector<Direction> generated;
    generated.reserve(6);
    for (int i = 0; i < 6; ++i) {
        const int index = (stressSeed_ + i) % static_cast<int>(pattern.size());
        generated.push_back(pattern[index]);
    }

    if (TryAcceptDirection(currentDirection, Direction::Up)) {
        generated.push_back(Direction::Up);
    }

    ++stressSeed_;
    return generated;
}
