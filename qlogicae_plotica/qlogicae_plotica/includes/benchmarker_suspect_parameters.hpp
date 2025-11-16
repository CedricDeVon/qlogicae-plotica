#pragma once

#include "benchmarker_result_data.hpp"

#include <string>
#include <functional>

namespace QLogicaePlotica
{
    struct BenchmarkerSuspectParameters
    {
        std::string title = "";

        std::string color_1 = "";

        std::string color_2 = "";

        std::function<void(BenchmarkerResultData&)> on_setup
            = [](BenchmarkerResultData& data) {};

        std::function<void(BenchmarkerResultData&)> on_execution
            = [](BenchmarkerResultData& data) {};
    };
}
