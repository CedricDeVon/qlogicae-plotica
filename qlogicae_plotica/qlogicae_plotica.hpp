#pragma once

#include "pch.h"

#include "utilities.hpp"
#include "qlogicae_core.hpp"

namespace QLogicaePlotica
{
    enum class BenchmarkerLegendAlignment : uint8_t
    {
        TOP,
        TOP_LEFT,
        TOP_RIGHT,
        RIGHT,
        BOTTOM,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        LEFT,
        CENTER
    };

    class BenchmarkerResultData
    {
    public:
        BenchmarkerResultData(const size_t& input_size);
        ~BenchmarkerResultData() = default;
        BenchmarkerResultData(
            const BenchmarkerResultData&) = delete;
        BenchmarkerResultData(
            BenchmarkerResultData&&) noexcept = default;
        BenchmarkerResultData& operator=(
            const BenchmarkerResultData&) = delete;
        BenchmarkerResultData& operator=(
            BenchmarkerResultData&&) noexcept = default;

        std::any& get_payload();
        size_t get_input_size() const;
        void set_end_execution_time();
        void set_start_execution_time();
        double get_setup_duration_time() const;
        double get_execution_duration_time() const;
        const std::any& get_payload() const;
        void set_payload(const std::any& value);
        double get_end_execution_time() const;
        double get_start_execution_time() const;
        double get_end_setup_time() const;
        double get_start_setup_time() const;
        void set_end_setup_time();
        void set_start_setup_time();
        template <typename Type> Type& get_payload();
        template <typename Type> void set_payload(Type&& value);
        template <typename Type> const Type& get_payload() const;

    private:
        std::any _payload;
        size_t _input_size;
        double _end_setup_time;
        double _start_setup_time;
        double _end_execution_time;
        double _start_execution_time;
    };

    template <typename Type>
    void BenchmarkerResultData::set_payload(Type&& value)
    {
        _payload = std::forward<Type>(value);
    }

    template <typename Type>
    Type& BenchmarkerResultData::get_payload()
    {
        return std::any_cast<Type&>(_payload);
    }

    template <typename Type>
    const Type& BenchmarkerResultData::get_payload() const
    {
        return std::any_cast<const Type&>(_payload);
    }

    struct BenchmarkerSuspectData
    {
        std::string title;
        std::string line_color;
        std::function<void(BenchmarkerResultData&)> on_setup;
        std::function<void(BenchmarkerResultData&)> on_execution;
    };

    struct BenchmarkerExecutionData
    {
        std::string title = "Title";
        std::string output_folder_path = "";
        size_t input_count = 1;
        size_t starting_input_count = 1;
        size_t incremental_input_count = 1;
        size_t ending_input_count = 100;
        size_t input_retry_count = 1;
        size_t warmup_iterations = 1;
        size_t maximum_output_count = 1'000'000;

        std::string x_title = "Input Size";
        std::string y_title = "Time";
        QLogicaeCore::TimeScaleUnit y_axis_time_scale_unit =
            QLogicaeCore::TimeScaleUnit::NANOSECONDS;
        BenchmarkerLegendAlignment legend_alignment = BenchmarkerLegendAlignment::TOP;
        bool is_line_color_set_to_default = true;
        bool is_gui_output_enabled = true;
        bool is_csv_output_enabled = false;
        bool is_json_output_enabled = false;
        bool is_jpg_output_enabled = false;
        bool is_html_output_enabled = false;
        bool is_svg_output_enabled = false;
        bool is_gif_output_enabled = false;
        bool is_txt_output_enabled = false;
        bool is_eps_output_enabled = false;
        bool is_tex_output_enabled = false;
        std::vector<BenchmarkerSuspectData> suspects;
    };

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
        static RuntimePerformanceBenchmarker& get_instance();

        BenchmarkerResult execute(const BenchmarkerExecutionData& data);
        std::future<BenchmarkerResult> execute_async(
            const BenchmarkerExecutionData& data);
        BenchmarkerResult execute_a(const BenchmarkerExecutionData& data);
        std::future<BenchmarkerResult> execute_a_async(
            const BenchmarkerExecutionData& data);

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

        std::vector<size_t> _generate_downsampled_indices(
            const size_t& total_points, const size_t& maximum_points) const;
        std::string _generate_matplot_output_file(
            const std::string& file_path,
            const std::string& extension_name);
        std::string _generate_matplot_output_directory_path(
            const std::string& file_path,
            const std::string& title);
        matplot::legend::general_alignment _get_benchmark_legend_alignment(
            const BenchmarkerLegendAlignment&) const;
    };
}
