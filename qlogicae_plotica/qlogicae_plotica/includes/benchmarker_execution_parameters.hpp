#pragma once

#include "benchmarker_graph.hpp"
#include "benchmarker_legend_alignment.hpp"
#include "benchmarker_suspect_parameters.hpp"

#include "qlogicae_core/includes/all.hpp"

namespace QLogicaePlotica
{
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
}
