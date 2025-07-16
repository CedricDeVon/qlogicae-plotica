#pragma once

#include "pch.h"

#include "utilities.hpp"
#include "qlogicae_core.hpp"

namespace QLogicaePlotica
{
    class BenchmarkerResultData
    {
    public:
        BenchmarkerResultData(const size_t& input_size);

        std::any& get_payload();
        uint64_t get_end_time() const;
        size_t get_input_size() const;
        uint64_t get_start_time() const;
        double get_duration_time() const;
        const std::any& get_payload() const;
        template <typename Type> Type& get_payload();
        template <typename Type> void set_payload(Type&& value);
        template <typename Type> const Type& get_payload() const;

        void set_end_time();
        void set_start_time();
        void set_payload(const std::any& value);

    private:
        std::any _payload;
        size_t _input_size;
        uint64_t _end_time;
        uint64_t _start_time;
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
        std::function<void(BenchmarkerResultData&)> on_setup;
        std::function<void(BenchmarkerResultData&)> on_execution;
    };

    struct BenchmarkerExecutionData
    {
        std::string title;
        size_t starting_input_count = 1;
        size_t incremental_input_count = 1;
        size_t ending_input_count = 1000;
        size_t input_retry_count = 1;
        size_t warmup_iterations = 0;
        size_t maximum_output_count = 1000;

        std::vector<BenchmarkerSuspectData> suspects;

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

        std::string _generate_output_file_path(
            const std::filesystem::path& file_path,
            const std::string& extension_name);
    };
}
