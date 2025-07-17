#pragma once

#include "pch.h"

#include "qlogicae_plotica.hpp"

using namespace indicators;
namespace QLogicaePlotica
{
    BenchmarkerResultData::BenchmarkerResultData(const size_t& input_size)
        : _input_size(input_size), _start_time(0), _end_time(0)
    {
    }

    size_t BenchmarkerResultData::get_input_size() const
    {
        return _input_size;
    }

    uint64_t BenchmarkerResultData::get_start_time() const
    {
        return _start_time;
    }

    uint64_t BenchmarkerResultData::get_end_time() const
    {
        return _end_time;
    }

    std::any& BenchmarkerResultData::get_payload()
    {
        return _payload;
    }

    const std::any& BenchmarkerResultData::get_payload() const
    {
        return _payload;
    }

    double BenchmarkerResultData::get_duration_time() const
    {
        return static_cast<double>(_end_time - _start_time);
    }

    void BenchmarkerResultData::set_start_time()
    {
        _start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now()
            .time_since_epoch()).count();
    }

    void BenchmarkerResultData::set_end_time()
    {
        _end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now()
            .time_since_epoch()).count();
    }

    void BenchmarkerResultData::set_payload(const std::any& value)
    {
        _payload = value;
    }

    RuntimePerformanceBenchmarker& RuntimePerformanceBenchmarker::
        get_instance()
    {
        static RuntimePerformanceBenchmarker get_instance;

        return get_instance;
    }

    std::vector<size_t> RuntimePerformanceBenchmarker::
        _generate_downsampled_indices(
            const size_t& total_points,
            const size_t& maximum_points) const
    {
        size_t index;
        std::vector<size_t> indices;
        if (total_points <= maximum_points)
        {
            for (index = 0; index < total_points; ++index)
            {
                indices.push_back(index);
            }
            return indices;
        }

        double step = static_cast<double>(
            total_points - 1) / (maximum_points - 1);
        for (index = 0; index < maximum_points; ++index)
        {
            indices.push_back(static_cast<size_t>(index * step));
        }

        return indices;
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute(
        const BenchmarkerExecutionData& data)
    {
        std::scoped_lock lock(_mutex);

        BenchmarkerResult result;

        if (data.title.empty() ||
            data.incremental_input_count < 1 ||
            data.input_retry_count == 0 ||
            data.ending_input_count < data.starting_input_count ||
            data.suspects.empty())
        {
            result.success = false;
            return result;
        }

        ProgressBar progress{
            option::BarWidth{50},
            option::Start{"["},
            option::Fill{"="},
            option::Lead{">"},
            option::Remainder{" "},
            option::End{"]"},
            option::ForegroundColor{Color::white},
            option::ShowPercentage{true},
            option::ShowElapsedTime{true},
            option::ShowRemainingTime{true}
        };

        std::atomic<size_t> completed_tasks = 0;
        size_t index_a, index_b, index_c, index_d;
        const size_t data_suspects_size = data.suspects.size();
        std::vector<size_t> full_input_sizes, downsample_indices;
        const size_t data_warmup_iterations = data.warmup_iterations;
        const size_t data_input_retry_count = data.input_retry_count;
        const size_t data_starting_input_count = data.starting_input_count;
        const size_t data_maximum_output_count = data.maximum_output_count;
        const size_t data_ending_input_count = data.ending_input_count + 1;
        const size_t data_incremental_input_count =
            data.incremental_input_count;
        std::vector<std::tuple<size_t, size_t, std::future<void>>>
            scheduled_tasks;
        
        for (index_a = data_starting_input_count;
            index_a < data_ending_input_count;
            index_a += data_incremental_input_count)
        {
            full_input_sizes.push_back(index_a);
        }

        downsample_indices = _generate_downsampled_indices(
            full_input_sizes.size(), data_maximum_output_count);
        result.input_sizes.reserve(downsample_indices.size());
        for (size_t index : downsample_indices)
        {
            result.input_sizes.push_back(
                static_cast<double>(full_input_sizes[index]));
        }
        result.suspect_names.reserve(data_suspects_size);
        result.durations.resize(data_suspects_size);
        index_c = data_suspects_size * result.input_sizes.size();        

        std::mutex result_mutex;
        for (index_a = 0; index_a < data_suspects_size; ++index_a)
        {
            const BenchmarkerSuspectData& suspect = data.suspects[index_a];
            result.suspect_names.push_back(suspect.title);
            result.durations[index_a].resize(result.input_sizes.size());

            for (index_b = 0; index_b < downsample_indices.size(); ++index_b)
            {
                index_d = full_input_sizes[downsample_indices[index_b]];
                std::optional<std::future<void>> future_opt = THREAD_POOL.enqueue_task(
                    [&, index_a, index_b, index_d]()
                    {
                        for (size_t index = 0;
                            index < data_warmup_iterations;
                            ++index)
                        {
                            BenchmarkerResultData benchmark_result_data(
                                index_d);
                            data.suspects[index_a].on_setup(
                                benchmark_result_data);
                            data.suspects[index_a].on_execution(
                                benchmark_result_data);
                        }

                        double average = 0.0;
                        for (size_t index = 0;
                            index < data.input_retry_count;
                            ++index)
                        {
                            BenchmarkerResultData benchmark_result_data(
                                index_d);
                            data.suspects[index_a].on_setup(
                                benchmark_result_data);
                            data.suspects[index_a].on_execution(
                                benchmark_result_data);

                            average += benchmark_result_data
                                .get_duration_time();
                        }
                        average = (average / static_cast<double>(
                            data_input_retry_count)) / 1'000'000'000.0;
                        {
                            std::lock_guard<std::mutex> lock(result_mutex);

                            result.durations[index_a][index_b] = average;
                        }

                        ++completed_tasks;
                        progress.set_progress(static_cast<size_t>(
                            (100.0 * completed_tasks.load()) / index_c));
                    });

                if (future_opt.has_value())
                {
                    scheduled_tasks.emplace_back(
                        index_a, index_b, std::move(future_opt.value()));
                }
            }
        }

        for (auto& [size, index, future] : scheduled_tasks)
        {
            future.get();
        }

        progress.mark_as_completed();

        if (data.is_gui_output_enabled)
        {
            matplot::colororder(DEFAULT_GUI_LINE_COLORS);
            matplot::plot(result.input_sizes, result.durations);
            matplot::legend_handle legends = matplot::legend(result.suspect_names);
            legends->location(matplot::legend::general_alignment::bottomleft);
            legends->box(false);

            matplot::grid(true);
            matplot::title(data.title);
            matplot::xlabel("Input Size");
            matplot::ylabel("Time (In Seconds)");
            matplot::show();
        }

        std::filesystem::path current_file_path = DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER;
        if (!std::filesystem::exists(current_file_path))
        {
            std::filesystem::create_directories(current_file_path);
        }

        current_file_path = DEFAULT_PROJECT_ROOT_OUTPUT_PATH;
        if (!std::filesystem::exists(current_file_path))
        {
            std::filesystem::create_directories(current_file_path);
        }

        current_file_path += "/" + data.title;
        if (!std::filesystem::exists(current_file_path))
        {
            std::filesystem::create_directories(current_file_path);
        }

        current_file_path += "/" + GENERATOR.random_uuid4();
        if (!std::filesystem::exists(current_file_path))
        {
            std::filesystem::create_directories(current_file_path);
        }

        if (data.is_svg_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "svg"));
        }

        if (data.is_eps_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "eps"));
        }

        if (data.is_gif_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "gif"));
        }

        if (data.is_jpg_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "jpg"));
        }

        if (data.is_html_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "html"));
        }

        if (data.is_txt_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "txt"));
        }

        if (data.is_tex_output_enabled)
        {
            matplot::save(_generate_output_file_path(current_file_path, "tex"));
        }

        return result;
    }

    std::future<BenchmarkerResult> RuntimePerformanceBenchmarker::execute_async(
        const BenchmarkerExecutionData& data)
    {
        return std::async(std::launch::async, [this, data]()
            {
                return execute(data);
            });
    }

    std::string RuntimePerformanceBenchmarker::_generate_output_file_path(
        const std::filesystem::path& file_path,
        const std::string& extension_name)
    {
        return std::filesystem::path(
            file_path.string() + "/" + DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER +
            "." + extension_name).string();
    }
}

