#include "utils.h"
#include <indicators/color.hpp>
#include <indicators/font_style.hpp>
#include <indicators/progress_spinner.hpp>
#include <indicators/setting.hpp>

indicators::ProgressSpinner PostfixSpinner(std::string postfix)
{
    namespace opt = indicators::option;
    using indicators::FontStyle;
    return indicators::ProgressSpinner {
        opt::PostfixText { postfix },
        opt::FontStyles { std::vector<FontStyle> { FontStyle::bold } },
        opt::ShowElapsedTime { true },
        opt::ShowPercentage { true },
        opt::ShowRemainingTime { true },
        opt::ShowSpinner { false },
        opt::ForegroundColor { indicators::Color::green },
    };
}
