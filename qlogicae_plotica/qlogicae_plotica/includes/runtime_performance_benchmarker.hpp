#pragma once

#include "benchmarker_graph.hpp"
#include "benchmarker_result.hpp"
#include "benchmarker_file_system.hpp"
#include "benchmarker_legend_alignment.hpp"
#include "benchmarker_suspect_parameters.hpp"
#include "benchmarker_execution_parameters.hpp"

#include "utilities.hpp"

namespace QLogicaePlotica
{       
    class RuntimePerformanceBenchmarker
    {
    public:
        BenchmarkerResult execute(
            BenchmarkerExecutionParameters data        
        );
        
        std::future<BenchmarkerResult> execute_async(
            BenchmarkerExecutionParameters data
        );

        static RuntimePerformanceBenchmarker& get_instance();

    protected:
        RuntimePerformanceBenchmarker() = default;
        
        ~RuntimePerformanceBenchmarker() = default;
        
        RuntimePerformanceBenchmarker(
            const RuntimePerformanceBenchmarker&
        ) = delete;
        
        RuntimePerformanceBenchmarker(
            RuntimePerformanceBenchmarker&&
        ) noexcept = default;
        
        RuntimePerformanceBenchmarker& operator=(
            const RuntimePerformanceBenchmarker&
        ) = delete;
        
        RuntimePerformanceBenchmarker& operator=(
            RuntimePerformanceBenchmarker&&
        ) noexcept = default;

        mutable std::mutex _mutex;

        bool _setup_file_system(
            BenchmarkerExecutionParameters& execution_data
        );
        
        bool _collect_results(
            BenchmarkerExecutionParameters& execution_data,
            BenchmarkerResult& execution_result
        );
        
        bool _transform_results(        
            BenchmarkerExecutionParameters& execution_data,
            BenchmarkerResult& execution_result
        );
        
        bool _output_results(
            BenchmarkerExecutionParameters& execution_data,
            BenchmarkerResult& execution_result
        );
        
        std::vector<size_t> _generate_downsampled_indices(
            const size_t& total_points, const size_t& maximum_points
        ) const;
    };
}
