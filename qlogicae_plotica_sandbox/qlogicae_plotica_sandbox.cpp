#pragma once

#include "pch.h"

#include "qlogicae_plotica_sandbox.hpp"

int main()
{
    QLogicaePlotica::RuntimePerformanceBenchmarker& benchmarker =
        QLogicaePlotica::RuntimePerformanceBenchmarker::get_instance();

    benchmarker.execute(
        {
            .title = "String Concatenations",
            .starting_input_count = 100'000,
            .incremental_input_count = 100'000,
            .ending_input_count = 10'000'000,
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
