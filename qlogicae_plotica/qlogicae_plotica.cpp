#pragma once

#include "pch.h"

#include "qlogicae_plotica.hpp"

namespace QLogicaePlotica
{
    BenchmarkerResultData::BenchmarkerResultData(const size_t& input_size) :
        _input_size(input_size),
        _start_execution_time(0),
        _end_execution_time(0)
    {
    }

    size_t BenchmarkerResultData::get_input_size() const
    {
        return _input_size;
    }

    double BenchmarkerResultData::get_start_execution_time() const
    {
        return _start_execution_time;
    }

    double BenchmarkerResultData::get_end_execution_time() const
    {
        return _end_execution_time;
    }

    std::any& BenchmarkerResultData::get_payload()
    {
        return _payload;
    }

    const std::any& BenchmarkerResultData::get_payload() const
    {
        return _payload;
    }

    double BenchmarkerResultData::get_setup_duration_time() const
    {
        return _end_setup_time - _start_setup_time;
    }

    double BenchmarkerResultData::get_execution_duration_time() const
    {
        return _end_execution_time - _start_execution_time;
    }

    void BenchmarkerResultData::set_start_execution_time()
    {
        _start_execution_time = TIME.now();
    }

    void BenchmarkerResultData::set_end_execution_time()
    {
        _end_execution_time = TIME.now();
    }

    double BenchmarkerResultData::get_end_setup_time() const
    {
        return _end_setup_time;
    }

    double BenchmarkerResultData::get_start_setup_time() const
    {
        return _start_setup_time;
    }

    void BenchmarkerResultData::set_start_setup_time()
    {
        _start_setup_time = TIME.now();
    }

    void BenchmarkerResultData::set_end_setup_time()
    {
        _end_setup_time = TIME.now();
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
        try
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
        catch (...)
        {
            return {};
        }
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute(
        const BenchmarkerExecutionData& execution_data)
    {
        BenchmarkerResult result;
        try
        {
            std::scoped_lock lock(_mutex);

            std::string execution_data_title =
                execution_data.title;
            std::string execution_data_x_title =
                execution_data.x_title;
            std::string execution_data_y_title =
                execution_data.y_title;
            std::string execution_data_output_folder_path =
                execution_data.output_folder_path;
            QLogicaeCore::TimeScaleUnit execution_data_y_axis_time_scale_unit =
                execution_data.y_axis_time_scale_unit;
            BenchmarkerLegendAlignment execution_data_legend_alignment =
                execution_data.legend_alignment;
            const bool execution_data_is_line_color_set_to_default =
                execution_data.is_line_color_set_to_default;
            const size_t execution_data_title_size =
                execution_data.title.size();
            const size_t execution_data_suspects_size =
                execution_data.suspects.size();
            const size_t execution_data_warmup_iterations =
                execution_data.warmup_iterations;
            const size_t execution_data_input_retry_count =
                execution_data.input_retry_count;
            const size_t execution_data_starting_input_count =
                execution_data.starting_input_count;
            const size_t execution_data_maximum_output_count =
                execution_data.maximum_output_count;
            const size_t execution_data_ending_input_count =
                execution_data.ending_input_count + 1;
            const size_t execution_data_incremental_input_count =
                execution_data.incremental_input_count;
            const bool execution_data_is_gui_output_enabled =
                execution_data.is_gui_output_enabled;
            const bool execution_data_is_csv_output_enabled =
                execution_data.is_csv_output_enabled;
            const bool execution_data_is_json_output_enabled =
                execution_data.is_json_output_enabled;
            const bool execution_data_is_jpg_output_enabled =
                execution_data.is_jpg_output_enabled;
            const bool execution_data_is_html_output_enabled =
                execution_data.is_html_output_enabled;
            const bool execution_data_is_svg_output_enabled =
                execution_data.is_svg_output_enabled;
            const bool execution_data_is_gif_output_enabled =
                execution_data.is_gif_output_enabled;
            const bool execution_data_is_txt_output_enabled =
                execution_data.is_txt_output_enabled;
            const bool execution_data_is_eps_output_enabled =
                execution_data.is_eps_output_enabled;
            const bool execution_data_is_tex_output_enabled =
                execution_data.is_tex_output_enabled;
            const bool execution_data_is_file_output_enabled = (
                execution_data_is_csv_output_enabled ||
                execution_data_is_json_output_enabled ||
                execution_data_is_jpg_output_enabled ||
                execution_data_is_html_output_enabled ||
                execution_data_is_svg_output_enabled ||
                execution_data_is_gif_output_enabled ||
                execution_data_is_txt_output_enabled ||
                execution_data_is_eps_output_enabled ||
                execution_data_is_tex_output_enabled);

            if ((!execution_data_is_gui_output_enabled &&
                !execution_data_is_file_output_enabled) ||
                !execution_data_title_size ||
                !execution_data_suspects_size ||
                execution_data_starting_input_count < 0 ||
                execution_data_incremental_input_count < 1 ||
                execution_data_input_retry_count < 0 ||
                execution_data_ending_input_count <
                execution_data_starting_input_count)
            {
                result.success = false;

                return result;
            }

            std::atomic<size_t> completed_tasks = 0;
            size_t index_a, index_b, index_c, index_d;
            std::vector<size_t> full_input_sizes, downsample_indices;
            std::vector<std::future<void>> output_futures;
            std::vector<std::tuple<size_t, size_t, std::future<void>>>
                scheduled_tasks;

            indicators::ProgressBar progress
            {
                indicators::option::BarWidth{50},
                indicators::option::Start{"["},
                indicators::option::Fill{"="},
                indicators::option::Lead{">"},
                indicators::option::Remainder{" "},
                indicators::option::End{"]"},
                indicators::option::ForegroundColor{indicators::Color::white},
                indicators::option::ShowPercentage{true},
                indicators::option::ShowElapsedTime{true},
                indicators::option::ShowRemainingTime{true}
            };

            for (index_a = execution_data_starting_input_count;
                index_a < execution_data_ending_input_count;
                index_a += execution_data_incremental_input_count)
            {
                full_input_sizes.push_back(index_a);
            }

            downsample_indices = _generate_downsampled_indices(
                full_input_sizes.size(), execution_data_maximum_output_count);
            result.input_sizes.reserve(downsample_indices.size());
            for (size_t index : downsample_indices)
            {
                result.input_sizes.push_back(
                    static_cast<double>(full_input_sizes[index]));
            }
            result.suspect_names.reserve(execution_data_suspects_size);
            result.durations.resize(execution_data_suspects_size);
            index_c = execution_data_suspects_size * result.input_sizes.size();

            std::mutex result_mutex;
            for (index_a = 0; index_a < execution_data_suspects_size; ++index_a)
            {
                const BenchmarkerSuspectData& suspect =
                    execution_data.suspects[index_a];
                result.suspect_names.push_back(suspect.title);
                result.durations[index_a].resize(result.input_sizes.size());

                for (index_b = 0; index_b < downsample_indices.size(); ++index_b)
                {
                    index_d = full_input_sizes[downsample_indices[index_b]];
                    std::optional<std::future<void>> future_opt =
                        THREAD_POOL.enqueue_task(
                            [&, index_a, index_b, index_d]()
                            {
                                for (size_t index = 0;
                                    index < execution_data_warmup_iterations;
                                    ++index)
                                {
                                    BenchmarkerResultData benchmark_result_execution_data(
                                        index_d);
                                    execution_data.suspects[index_a].on_setup(
                                        benchmark_result_execution_data);
                                    execution_data.suspects[index_a].on_execution(
                                        benchmark_result_execution_data);
                                }

                                double execution_average = 0.0;
                                for (size_t index = 0;
                                    index < execution_data_input_retry_count;
                                    ++index)
                                {
                                    BenchmarkerResultData benchmark_result_execution_data(
                                        index_d);
                                    execution_data.suspects[index_a].on_setup(
                                        benchmark_result_execution_data);
                                    execution_data.suspects[index_a].on_execution(
                                        benchmark_result_execution_data);

                                    execution_average += benchmark_result_execution_data
                                        .get_execution_duration_time();
                                }

                                execution_average = TIME.convert_nanoseconds(
                                    (execution_average / static_cast<double>(
                                        execution_data_input_retry_count)),
                                    execution_data_y_axis_time_scale_unit
                                );
                                {
                                    std::lock_guard<std::mutex> lock(result_mutex);

                                    result.durations[index_a][index_b] =
                                        execution_average;
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

            if (execution_data_is_gui_output_enabled)
            {
                std::future<void> execution_data_is_gui_output_future = 
                    std::async(std::launch::async, [=]() mutable
                {
                    std::vector<std::string> line_colors;
                    if (execution_data_is_line_color_set_to_default)
                    {
                        line_colors = DEFAULT_GUI_LINE_COLORS;
                    }
                    else
                    {
                        for (const auto& suspect : execution_data.suspects)
                        {
                            line_colors.push_back(suspect.line_color);
                        }
                    }

                    matplot::gcf()->name(execution_data_title);
                    matplot::colororder(line_colors);
                    matplot::plot(result.input_sizes, result.durations);
                    matplot::legend_handle legends = matplot::legend(
                        result.suspect_names);
                    legends->location(
                        _get_benchmark_legend_alignment(
                            execution_data_legend_alignment
                        )
                    );
                    legends->box(false);
                    matplot::grid(true);
                    matplot::gca()->minor_grid(true);
                    matplot::title(execution_data_title);
                    matplot::xlabel(execution_data_x_title);
                    matplot::ylabel(execution_data_y_title +
                        " In " + TIME.get_time_unit_full_name(
                            execution_data_y_axis_time_scale_unit
                        ).data()
                    );
                    matplot::show();
                });
            }

            if (execution_data_is_file_output_enabled)
            {
                std::string current_file_path =
                    _generate_matplot_output_directory_path(
                        execution_data_output_folder_path,
                        execution_data_title
                    );
                std::unordered_map<std::string, bool> matplot_defined_outputs;
                matplot_defined_outputs["svg"] =
                    execution_data_is_svg_output_enabled;
                matplot_defined_outputs["eps"] =
                    execution_data_is_eps_output_enabled;
                matplot_defined_outputs["gif"] =
                    execution_data_is_gif_output_enabled;
                matplot_defined_outputs["jpg"] =
                    execution_data_is_jpg_output_enabled;
                matplot_defined_outputs["html"] =
                    execution_data_is_html_output_enabled;
                matplot_defined_outputs["txt"] =
                    execution_data_is_txt_output_enabled;
                matplot_defined_outputs["tex"] =
                    execution_data_is_tex_output_enabled;

                for (const auto& [file_name, is_enabled] :
                    matplot_defined_outputs)
                {
                    if (is_enabled)
                    {
                        output_futures.emplace_back(
                            std::async(std::launch::async, [=]()
                            {
                                matplot::save(
                                    _generate_matplot_output_file(
                                        current_file_path,
                                        file_name
                                    )
                                );
                            })
                        );
                    }
                }

                if (execution_data_is_csv_output_enabled)
                {
                    output_futures.emplace_back(
                        std::async(std::launch::async, [=]() mutable
                        {
                            CSV_FILE_IO.set_file_path(
                                _generate_matplot_output_file(
                                    current_file_path, "csv"
                                )
                            );

                            std::string output = "Input Size";
                            for (const std::string suspect_name :
                                result.suspect_names)
                            {
                                output += "," + suspect_name;
                            }
                            output += "\n";

                            size_t result_input_size =
                                    result.input_sizes.size(),
                                result_suspect_size =
                                    result.suspect_names.size(),
                                result_suspect_row_size =
                                    result_suspect_size + 1;
                            for (index_a = 0;
                                index_a < result_input_size;
                                ++index_a)
                            {
                                output +=
                                    absl::StrCat(result.input_sizes[index_a]);
                                for (index_b = 0;
                                    index_b < result_suspect_size;
                                    ++index_b)
                                {
                                    output += absl::StrCat(
                                        ",",
                                        result.durations[index_b][index_a]
                                    );
                                }
                                output += "\n";
                            }
                            CSV_FILE_IO.write_async(output);
                        })
                    );
                }

                for (auto& output_future : output_futures)
                {
                    output_future.get();
                }
            }

            progress.mark_as_completed();

            return result;
        }
        catch (...)
        {
            return result;
        }        
    }

    std::future<BenchmarkerResult> RuntimePerformanceBenchmarker::execute_async(
        const BenchmarkerExecutionData& execution_data)
    {
        return std::async(std::launch::async, [this, execution_data]()
            {
                return execute(execution_data);
            });
    }

    std::string RuntimePerformanceBenchmarker::
        _generate_matplot_output_directory_path(
            const std::string& file_path,
            const std::string& title)
    {
        try
        {
            std::string complete_matplot_output_directory_path =
                title + "/" + TIME.now(
                    QLogicaeCore::TimeFormat::FULL_DASHED_TIMESTAMP
                );
            complete_matplot_output_directory_path =
                (file_path.empty()) ?
                    DEFAULT_PROJECT_ROOT_OUTPUT_PATH + "/" +
                    complete_matplot_output_directory_path
                 :
                    file_path + "/" +
                    complete_matplot_output_directory_path;
            if (!std::filesystem::exists(
                complete_matplot_output_directory_path))
            {
                std::filesystem::create_directories(
                    complete_matplot_output_directory_path);
            }

            return complete_matplot_output_directory_path;
        }
        catch (...)
        {
            return "";
        }
    }

    std::string RuntimePerformanceBenchmarker::_generate_matplot_output_file(
        const std::string& file_path,
        const std::string& extension_name)
    {
        try
        {
            return file_path + "/" +
                DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER + "." +
                extension_name;
        }
        catch (...)
        {
            return "";
        }
    }

    matplot::legend::general_alignment RuntimePerformanceBenchmarker::
        _get_benchmark_legend_alignment(
            const BenchmarkerLegendAlignment& benchmarker_legend_alignment
        ) const
    {
        switch (benchmarker_legend_alignment)
        {
        case BenchmarkerLegendAlignment::LEFT:
            return matplot::legend::general_alignment::left;

        case BenchmarkerLegendAlignment::RIGHT:
            return matplot::legend::general_alignment::right;

        case BenchmarkerLegendAlignment::CENTER:
            return matplot::legend::general_alignment::center;

        case BenchmarkerLegendAlignment::TOP:
            return matplot::legend::general_alignment::top;

        case BenchmarkerLegendAlignment::TOP_LEFT:
            return matplot::legend::general_alignment::topleft;

        case BenchmarkerLegendAlignment::TOP_RIGHT:
            return matplot::legend::general_alignment::topright;

        case BenchmarkerLegendAlignment::BOTTOM:
            return matplot::legend::general_alignment::bottom;

        case BenchmarkerLegendAlignment::BOTTOM_LEFT:
            return matplot::legend::general_alignment::bottomleft;

        case BenchmarkerLegendAlignment::BOTTOM_RIGHT:
            return matplot::legend::general_alignment::bottomright;

        default:
            return matplot::legend::general_alignment::topright;
        }
    }
}
