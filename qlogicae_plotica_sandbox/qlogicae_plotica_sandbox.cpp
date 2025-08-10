#pragma once

#include "pch.h"

#include "qlogicae_plotica_sandbox.hpp"

int main()
{
    QLogicaePlotica::RuntimePerformanceBenchmarker& benchmarker =
        QLogicaePlotica::RuntimePerformanceBenchmarker::get_instance();

    benchmarker.execute_a(
        {
            .title = "String Concatenations",
            .input_count = 1'000'000,
            .input_retry_count = 100,
            .warmup_iterations = 2,
            .y_axis_time_scale_unit =
                QLogicaeCore::TimeScaleUnit::SECONDS,
            .is_csv_output_enabled = true,
            .is_jpg_output_enabled = true,      
            .suspects =
            {
                {
                    .title = "+= (with reserve())",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        size_t ia, sa = data.get_input_size();
                        std::string vb;
                        vb.reserve(sa);
                        for (ia = 0; ia < sa; ++ia)
                        {
                            vb += "0";
                        }
                        data.set_end_execution_time();
                    }
                },
                {
                    .title = "append()",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        size_t ia, sa = data.get_input_size();
                        std::string vb;
                        vb.reserve(sa);
                        for (ia = 0; ia < sa; ++ia)
                        {
                            vb.append("0");
                        }
                        data.set_end_execution_time();
                    }
                }
            }            
        }
    );
    
    return 0;
}

/*
* ,
                {
                    .title = "+= (without reserve())",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        std::string vb = "";
                        size_t ia, sa = data.get_input_size();
                        for (ia = 0; ia < sa; ++ia)
                        {
                            vb += "0";
                        }
                        data.set_end_execution_time();
                    }
                },
                {
                    .title = "absl::StrCat()",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        std::string vb = "";
                        size_t ia, sa = data.get_input_size();
                        for (ia = 0; ia < sa; ++ia)
                        {
                            vb += absl::StrCat("0");
                        }
                        data.set_end_execution_time();
                    }
                },
std::vector<std::vector<double>> mpg = {
        {1, 2, 3, 4, 5},
        {1, 2, 3, 4, 10},
        {1, 2, 3, 4, 5},
        {1, 2, 3, 4, 5},
        {1, 2, 3, 4, 5}
    };

    std::vector<std::string> origin = {
        "USA", "JAPAN", "Germany", "FRANCE", "SWEDEN"
    };

    auto ax = matplot::gca();
    ax->boxplot(mpg);
    ax->xticklabels(origin);
    matplot::xlabel("All Vehicles");
    matplot::ylabel("Miles per Gallon (MPG)");
    matplot::title("Miles per Gallon for All Vehicles");

    matplot::show();
*/