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
        .suspects =
        {
            {
                .title = "+=",
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
                .title = "absl::StrCat()",
                .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                {
                    data.set_start_execution_time();
                    size_t ia, sa = data.get_input_size();
                    std::string vb;
                    vb.reserve(sa);
                    for (ia = 0; ia < sa; ++ia)
                    {
                        vb += absl::StrCat("0");
                    }
                    data.set_end_execution_time();
                }
            }
        }
    });
    
    return 0;
}
