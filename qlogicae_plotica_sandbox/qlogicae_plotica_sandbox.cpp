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
                .title = "+= 1",
                .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                {
                    data.set_start_execution_time();
                    size_t ia, sa = data.get_input_size();
                    std::string vb;
                    vb.reserve(sa);
                    for (ia = 0; ia < sa; ++ia)
                    {
                        vb += "/";
                    }
                    data.set_end_execution_time();
                }
            },
            {
                .title = "+= 2",
                .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                {
                    data.set_start_execution_time();
                    size_t ia, sa = data.get_input_size();
                    std::string vb;
                    vb.reserve(sa);
                    for (ia = 0; ia < sa; ++ia)
                    {
                        vb += "//";
                    }
                    data.set_end_execution_time();
                }
            }
        }
    });
    
    return 0;
}
