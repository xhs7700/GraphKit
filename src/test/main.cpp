#include "graphkit.h"
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>

int main(int argc, char** argv)
{
    auto cur_path = std::filesystem::current_path();
    fmt::println("current path is {}.", cur_path.string());
    auto tmp = PROJECT_DIR;
    return 0;
}
