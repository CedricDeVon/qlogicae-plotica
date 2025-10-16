#pragma once

namespace QLogicaePlotica
{
    struct BenchmarkerResult
    {
        bool success = true;

        std::vector<double> input_sizes;

        std::vector<std::string> suspect_names;

        std::vector<std::vector<double>> durations;
    };
}
