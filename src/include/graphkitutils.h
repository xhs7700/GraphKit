#pragma once
#include <chrono>
#include <indicators/progress_spinner.hpp>
#include <string>

struct SetSpinner {
    indicators::ProgressSpinner spinner;
    std::uint64_t maxProgress;
    std::uint64_t curPercent;
    std::string postfixText;
    SetSpinner(std::string postfixText, std::uint64_t maxProgress);
    void setProgress(std::uint64_t curProgress);
    void markAsCompleted() { spinner.mark_as_completed(); }
};

struct TickSpinner {
    indicators::ProgressSpinner spinner;
    std::uint64_t maxProgress;
    std::uint64_t curProgress;
    std::uint64_t curPercent;
    std::string postfixText;
    TickSpinner(std::string postfixText, std::uint64_t maxProgress);
    void tick(std::uint64_t tickNum = 1);
    void markAsCompleted() { spinner.mark_as_completed(); }
};

struct UnknownSpinner {
    indicators::ProgressSpinner spinner;
    std::string postfixText;
    std::chrono::milliseconds latency;
    std::chrono::time_point<std::chrono::steady_clock> prevTime;
    UnknownSpinner(std::string postfixText, std::chrono::milliseconds latency);
    void tick();
    void markAsCompleted()
    {
        spinner.set_progress(100);
        spinner.mark_as_completed();
    }
};