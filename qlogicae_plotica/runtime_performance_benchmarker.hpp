#pragma once

#include "pch.h"

#include "utilities.hpp"
#include "qlogicae_core.hpp"

#include "benchmarker_file_system.hpp"
#include "benchmarker_result_data.hpp"

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

    struct BenchmarkerExecutionParameters
    {
        bool is_execution_enabled = true;
        bool is_configuration_parsing_enabled = true;
        bool is_data_transform_enabled = false;
        std::string title = "Title";
        BenchmarkerGraph gnuplot_type = BenchmarkerGraph::BOXPLOT;
        size_t static_input_count = 100;
        size_t static_iteration_count = 10;
        size_t starting_input_count = 1;
        size_t incremental_input_count = 1;
        size_t ending_input_count = 100;
        size_t input_retry_count = 1;
        size_t warmup_input_count = 1;
        size_t maximum_output_count = 1'000'000;
        QLogicaeCore::OutlierRemovalOptions outlier_removal_options =
        {
            .factor = 1.5,
            .threshold = 3.0,
            .proportion = 0.26,
            .significance_level = 0.05
        };
        std::string x_title = "Input Size";
        std::string y_title = "Time";
        BenchmarkerLegendAlignment legend_alignment =
            BenchmarkerLegendAlignment::TOP;
        QLogicaeCore::TimeScaleUnit y_axis_time_scale_unit =
            QLogicaeCore::TimeScaleUnit::SECONDS;
        bool is_file_output_enabled = true;
        bool is_default_line_colors_enabled = true;
        bool is_gnuplot_output_enabled = true;
        bool is_csv_output_enabled = false;
        bool is_json_output_enabled = false;
        bool is_jpg_output_enabled = false;
        bool is_html_output_enabled = false;
        bool is_svg_output_enabled = false;
        bool is_gif_output_enabled = false;
        bool is_txt_output_enabled = false;
        bool is_eps_output_enabled = false;
        bool is_tex_output_enabled = false;
        std::string output_folder_path = UTILITIES.QLOGICAE_DOT_QLOGICAE_PLOTICA_PATH;
        std::vector<BenchmarkerSuspectParameters> suspects;
    };

    static BenchmarkerExecutionParameters
        default_benchmarker_execution_parameters;

    struct BenchmarkerResult
    {
        bool success = true;
        std::vector<double> input_sizes;
        std::vector<std::string> suspect_names;
        std::vector<std::vector<double>> durations;
    };

    class RuntimePerformanceBenchmarker
    {
    public:
        BenchmarkerResult execute(BenchmarkerExecutionParameters data);
        std::future<BenchmarkerResult> execute_async(
            BenchmarkerExecutionParameters data);

        static RuntimePerformanceBenchmarker& get_instance();

    protected:
        RuntimePerformanceBenchmarker() = default;
        ~RuntimePerformanceBenchmarker() = default;
        RuntimePerformanceBenchmarker(
            const RuntimePerformanceBenchmarker&) = delete;
        RuntimePerformanceBenchmarker(
            RuntimePerformanceBenchmarker&&) noexcept = default;
        RuntimePerformanceBenchmarker& operator=(
            const RuntimePerformanceBenchmarker&) = delete;
        RuntimePerformanceBenchmarker& operator=(
            RuntimePerformanceBenchmarker&&) noexcept = default;

        mutable std::mutex _mutex;

        bool _setup_file_system(
            BenchmarkerExecutionParameters& execution_data);
        bool _collect_results(
            BenchmarkerExecutionParameters& execution_data,
            BenchmarkerResult& execution_result);
        bool _transform_results(
            BenchmarkerExecutionParameters& execution_data,
            BenchmarkerResult& execution_result);
        bool _output_results(
            BenchmarkerExecutionParameters& execution_data,
            BenchmarkerResult& execution_result);
        std::vector<size_t> _generate_downsampled_indices(
            const size_t& total_points, const size_t& maximum_points) const;
    };
}
