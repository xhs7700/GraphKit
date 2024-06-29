#include "graphkitutils.h"
#include <chrono>
#include <indicators/color.hpp>
#include <indicators/font_style.hpp>
#include <indicators/progress_spinner.hpp>
#include <indicators/setting.hpp>

SetSpinner::SetSpinner(std::string postfixText, std::uint64_t maxProgress)
    : maxProgress(maxProgress)
    , postfixText(postfixText)
    , curPercent(0)
    , spinner(
          indicators::option::PostfixText { postfixText },
          indicators::option::FontStyles { std::vector<indicators::FontStyle> { indicators::FontStyle::bold } },
          indicators::option::ShowElapsedTime { true },
          indicators::option::ShowPercentage { true },
          indicators::option::ShowRemainingTime { true },
          indicators::option::ShowSpinner { false },
          indicators::option::ForegroundColor { indicators::Color::green })
{
}
void SetSpinner::setProgress(std::uint64_t newProgress)
{
    std::uint64_t newPercent = static_cast<double>(newProgress) / static_cast<double>(maxProgress) * 100;
    if (newPercent > curPercent) {
        curPercent = newPercent;
        spinner.set_progress(newPercent);
    }
}

TickSpinner::TickSpinner(std::string postfixText, std::uint64_t maxProgress)
    : maxProgress(maxProgress)
    , postfixText(postfixText)
    , curProgress(0)
    , curPercent(0)
    , spinner(
          indicators::option::PostfixText { postfixText },
          indicators::option::FontStyles { std::vector<indicators::FontStyle> { indicators::FontStyle::bold } },
          indicators::option::ShowElapsedTime { true },
          indicators::option::ShowPercentage { true },
          indicators::option::ShowRemainingTime { true },
          indicators::option::ShowSpinner { false },
          indicators::option::ForegroundColor { indicators::Color::green })
{
}
void TickSpinner::tick(std::uint64_t tickNum)
{
    curProgress += tickNum;
    std::uint64_t newPercent = static_cast<double>(curProgress) / static_cast<double>(maxProgress) * 100;
    if (newPercent > curPercent) {
        curPercent = newPercent;
        spinner.set_progress(newPercent);
    }
}

UnknownSpinner::UnknownSpinner(std::string postfixText, std::chrono::milliseconds latency)
    : postfixText(postfixText)
    , latency(latency)
    , prevTime(std::chrono::steady_clock::now())
    , spinner(
          indicators::option::PostfixText { postfixText },
          indicators::option::FontStyles { std::vector<indicators::FontStyle> { indicators::FontStyle::bold } },
          indicators::option::ShowElapsedTime { true },
          indicators::option::ShowPercentage { false },
          indicators::option::ShowRemainingTime { false },
          indicators::option::ShowSpinner { false },
          indicators::option::ForegroundColor { indicators::Color::green })
{
    spinner.set_progress(0);
}
void UnknownSpinner::tick()
{
    const auto curTime = std::chrono::steady_clock::now();
    if (curTime - prevTime > latency) {
        spinner.set_progress(0);
        prevTime = curTime;
    }
}