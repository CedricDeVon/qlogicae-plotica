#include "pch.hpp"

#include "../includes/runtime_performance_benchmarker.hpp"

namespace QLogicaePlotica
{
    RuntimePerformanceBenchmarker& RuntimePerformanceBenchmarker::
        get_instance()
    {
        static RuntimePerformanceBenchmarker instance;

        return instance;
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute(
        BenchmarkerExecutionParameters execution_data
    )
    {
        try
        {
            std::scoped_lock lock(_mutex);

            BenchmarkerResult execution_result;

            UTILITIES.CLI_IO.print_with_new_line(
                UTILITIES.get_full_name()
            );

            if (!_setup_file_system(execution_data) ||
                !_collect_results(execution_data, execution_result) ||
                !_transform_results(execution_data, execution_result) ||
                !_output_results(execution_data, execution_result))
            {
                execution_result.success = false;

                return execution_result;
            }

            return execution_result;
        }
        catch (const std::exception& exception)
        {
            BenchmarkerResult execution_result;
            UTILITIES.LOGGER.log_with_timestamp(
                std::string("RuntimePerformanceBenchmarker::execute(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            execution_result.success = false;

            return execution_result;
        }
    }

    std::future<BenchmarkerResult>
        RuntimePerformanceBenchmarker::execute_async(
            BenchmarkerExecutionParameters execution_data
        )
    {
        return std::async(std::launch::async, [this, execution_data]()
            {
                return execute(execution_data);
            }
        );
    }

    std::vector<size_t> RuntimePerformanceBenchmarker::
        _generate_downsampled_indices(
            const size_t& total_points,
            const size_t& maximum_points
    ) const
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
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_with_timestamp(
                std::string("RuntimePerformanceBenchmarker::_generate_downsampled_indices(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return {};
        }
    }

    bool RuntimePerformanceBenchmarker::_setup_file_system(
        BenchmarkerExecutionParameters& execution_data
    )
    {
        try
        {
            indicators::ProgressBar progress_1
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
                indicators::option::PrefixText{ "> Setting Up File System: " },
                indicators::option::ShowRemainingTime{true}
            };

            execution_data.output_folder_path =
                BenchmarkerFileSystem::generate_root_folder(
                    execution_data.output_folder_path,
                    execution_data.title
                );

            if (execution_data.is_configuration_parsing_enabled)
            {
                size_t index_a,
                    size_b = execution_data.suspects.size(),
                    size_a = size_b - 1,
                    size_c,
                    size_d,
                    size_e;
                std::string configuration_file_path =
                    execution_data.output_folder_path +
                    "/" +
                    UTILITIES.RELATIVE_PLOTICA_CONFIGURATIONS_FILE_PATH;
                for (BenchmarkerSuspectParameters& suspect :
                    execution_data.suspects)
                {
                    if (suspect.title.empty())
                    {
                        suspect.title = UTILITIES.GENERATOR.random_uuid4();
                    }
                }

                UTILITIES.JSON_FILE_IO.set_file_path(configuration_file_path);
                if (!std::filesystem::exists(configuration_file_path))
                {
                    std::string suspects_json_string = "",
                        suspects_titles_json_string = "\t\t \"titles\": [\n",
                        suspects_color_1s_json_string = "\t\t \"color_1s\": [\n",
                        suspects_color_2s_json_string = "\t\t \"color_2s\": [\n";

                    for (index_a = 0; index_a < size_b; ++index_a)
                    {
                        BenchmarkerSuspectParameters& benchmarker_suspect_data = execution_data.suspects[index_a];

                        suspects_titles_json_string += "\t\t\t\"" + benchmarker_suspect_data.title + "\"";
                        suspects_color_1s_json_string += "\t\t\t\"" + benchmarker_suspect_data.color_1 + "\"";
                        suspects_color_2s_json_string += "\t\t\t\"" + benchmarker_suspect_data.color_2 + "\"";

                        if (index_a < size_a)
                        {
                            suspects_titles_json_string += ",";
                            suspects_color_1s_json_string += ",";
                            suspects_color_2s_json_string += ",";
                        }
                        suspects_titles_json_string += "\n";
                        suspects_color_1s_json_string += "\n";
                        suspects_color_2s_json_string += "\n";
                    }
                    suspects_json_string =
                        suspects_titles_json_string + "\t\t],\n" +
                        suspects_color_1s_json_string + "\t\t],\n" +
                        suspects_color_2s_json_string + "\t\t]\n";

                    std::string json_string = std::string() +
                        "{\n" +
                        "\t\"benchmarks\": {\n" +
                        "\t\t\"static_input_count\": " + absl::StrCat(execution_data.static_input_count) + ",\n" +
                        "\t\t\"static_iteration_count\": " + absl::StrCat(execution_data.static_iteration_count) + ",\n" +
                        "\t\t\"starting_input_count\": " + absl::StrCat(execution_data.starting_input_count) + ",\n" +
                        "\t\t\"incremental_input_count\": " + absl::StrCat(execution_data.incremental_input_count) + ",\n" +
                        "\t\t\"ending_input_count\": " + absl::StrCat(execution_data.ending_input_count) + ",\n" +
                        "\t\t\"input_retry_count\": " + absl::StrCat(execution_data.input_retry_count) + ",\n" +
                        "\t\t\"warmup_input_count\": " + absl::StrCat(execution_data.warmup_input_count) + ",\n" +
                        "\t\t\"maximum_output_count\": " + absl::StrCat(execution_data.maximum_output_count) + "\n" +
                        "\t},\n" +
                        "\t\"gnuplot\": {\n" +
                        "\t\t\"title\": \"" + execution_data.title + "\",\n" +
                        "\t\t\"type\": \"" + UTILITIES.BENCHMARKER_GRAPH_ENUMS.at(execution_data.gnuplot_type) + "\",\n" +
                        "\t\t\"x_title\": \"" + execution_data.x_title + "\",\n" +
                        "\t\t\"y_title\": \"" + execution_data.y_title + "\",\n" +
                        "\t\t\"legend_alignment\": \"" + UTILITIES.BENCHMARKER_LEGEND_ALIGNMENT_ENUMS.at(execution_data.legend_alignment) + "\",\n" +
                        "\t\t\"y_axis_time_scale_unit\": \"" + UTILITIES.TIME.get_time_unit_abbreviation(execution_data.y_axis_time_scale_unit).data() + "\"\n" +
                        "\t},\n" +
                        "\t\"is_enabled\": {\n" +
                        "\t\t\"execution\": " + (execution_data.is_execution_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"file_output\": " + (execution_data.is_file_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"default_line_colors\": " + (execution_data.is_default_line_colors_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"gnuplot_output\": " + (execution_data.is_gnuplot_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"csv_output\": " + (execution_data.is_csv_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"json_output\": " + (execution_data.is_json_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"jpg_output\": " + (execution_data.is_jpg_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"html_output\": " + (execution_data.is_html_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"svg_output\": " + (execution_data.is_svg_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"gif_output\": " + (execution_data.is_gif_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"txt_output\": " + (execution_data.is_txt_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"eps_output\": " + (execution_data.is_eps_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"tex_output\": " + (execution_data.is_tex_output_enabled ? "true" : "false") + "\n" +
                        "\t},\n" +
                        "\t\"suspects\": {\n" +
                        suspects_json_string +
                        "\t}\n" +
                        "}\n";

                    UTILITIES.JSON_FILE_IO.write_async(json_string);
                }
                else
                {
                    execution_data.static_input_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "static_input_count" }));
                    execution_data.static_iteration_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "static_iteration_count" }));
                    execution_data.starting_input_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "starting_input_count" }));
                    execution_data.incremental_input_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "incremental_input_count" }));
                    execution_data.ending_input_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "ending_input_count" }));
                    execution_data.input_retry_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "input_retry_count" }));
                    execution_data.warmup_input_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "warmup_input_count" }));
                    execution_data.maximum_output_count = static_cast<size_t>(UTILITIES.JSON_FILE_IO.get_double({ "benchmarks", "maximum_output_count" }));

                    execution_data.title = UTILITIES.JSON_FILE_IO.get_string({ "gnuplot", "title" });
                    execution_data.gnuplot_type = UTILITIES.BENCHMARKER_GRAPH_STRINGS.at(UTILITIES.JSON_FILE_IO.get_string({ "gnuplot", "type" }));
                    execution_data.x_title = UTILITIES.JSON_FILE_IO.get_string({ "gnuplot", "x_title" });
                    execution_data.y_title = UTILITIES.JSON_FILE_IO.get_string({ "gnuplot", "y_title" });
                    execution_data.legend_alignment = UTILITIES.BENCHMARKER_LEGEND_ALIGNMENT_STRINGS.at(UTILITIES.JSON_FILE_IO.get_string({ "gnuplot", "legend_alignment" }));
                    execution_data.y_axis_time_scale_unit = UTILITIES.TIME.get_time_unit_abbreviation(UTILITIES.JSON_FILE_IO.get_string({ "gnuplot", "y_axis_time_scale_unit" }));

                    execution_data.is_execution_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "execution" });
                    execution_data.is_file_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "file_output" });
                    execution_data.is_default_line_colors_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "default_line_colors" });
                    execution_data.is_gnuplot_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "gnuplot_output" });
                    execution_data.is_csv_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "csv_output" });
                    execution_data.is_json_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "json_output" });
                    execution_data.is_jpg_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "jpg_output" });
                    execution_data.is_html_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "html_output" });
                    execution_data.is_svg_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "svg_output" });
                    execution_data.is_gif_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "gif_output" });
                    execution_data.is_txt_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "txt_output" });
                    execution_data.is_eps_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "eps_output" });
                    execution_data.is_tex_output_enabled = UTILITIES.JSON_FILE_IO.get_bool({ "is_enabled", "tex_output" });

                    std::vector<std::any> titles = UTILITIES.JSON_FILE_IO.get_array({ "suspects", "titles" });
                    std::vector<std::any> color_1s = UTILITIES.JSON_FILE_IO.get_array({ "suspects", "color_1s" });
                    std::vector<std::any> color_2s = UTILITIES.JSON_FILE_IO.get_array({ "suspects", "color_2s" });
                    size_c = titles.size();
                    size_d = color_1s.size();
                    size_e = color_2s.size();

                    if (size_c != size_d || size_c != size_e)
                    {
                        UTILITIES.LOGGER.log_with_timestamp(
                            std::string("RuntimePerformanceBenchmarker::_setup_file_system(): 'suspects.titles', 'suspects.color_1s', and 'suspects.color_2s' array properties must be of equal size"),
                            QLogicaeCore::LogLevel::EXCEPTION
                        );

                        return false;
                    }
                    for (index_a = 0; index_a < size_c; ++index_a)
                    {
                        BenchmarkerSuspectParameters& benchmarker_suspect_data = execution_data.suspects[index_a];

                        benchmarker_suspect_data.title = std::any_cast<std::string>(titles[index_a]);
                        benchmarker_suspect_data.color_1 = std::any_cast<std::string>(color_1s[index_a]);
                        benchmarker_suspect_data.color_2 = std::any_cast<std::string>(color_2s[index_a]);
                    }
                }
            }

            progress_1.set_progress(static_cast<size_t>(100.0));

            return true;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_with_timestamp(
                std::string("RuntimePerformanceBenchmarker::_setup_file_system(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return false;
        }
    }

    bool RuntimePerformanceBenchmarker::_collect_results(
        BenchmarkerExecutionParameters& execution_data,
        BenchmarkerResult& execution_result
    )
    {
        try
        {
            std::mutex result_mutex;
            std::atomic<size_t> completed_tasks = 0;
            size_t index_a, index_b, index_c, index_d,
                execution_data_suspects_size =
                execution_data.suspects.size(),
                full_input_size,
                downsample_indices_size,
                input_sizes_size;
            std::vector<size_t> full_input_sizes, downsample_indices;
            std::vector<std::tuple<size_t, size_t, std::future<void>>>
                scheduled_tasks;

            switch (execution_data.gnuplot_type)
            {
            case BenchmarkerGraph::BOXPLOT:
                for (index_a = 0;
                    index_a < execution_data.static_iteration_count;
                    ++index_a)
                {
                    full_input_sizes.push_back(
                        execution_data.static_input_count
                    );
                }
                break;

            case BenchmarkerGraph::LINEPLOT:
                for (index_a = execution_data.starting_input_count;
                    index_a < execution_data.ending_input_count;
                    index_a += execution_data.incremental_input_count)
                {
                    full_input_sizes.push_back(index_a);
                }
                break;

            default:
                for (index_a = execution_data.starting_input_count;
                    index_a < execution_data.ending_input_count;
                    index_a += execution_data.incremental_input_count)
                {
                    full_input_sizes.push_back(index_a);
                }
                break;
            };
            full_input_size = full_input_sizes.size();

            downsample_indices = _generate_downsampled_indices(
                full_input_size, execution_data.maximum_output_count);
            downsample_indices_size = downsample_indices.size();

            execution_result.input_sizes.reserve(downsample_indices_size);
            for (size_t index : downsample_indices)
            {
                execution_result.input_sizes.push_back(
                    static_cast<double>(full_input_sizes[index]));
            }
            input_sizes_size = execution_result.input_sizes.size();
            execution_result.suspect_names.reserve(
                execution_data_suspects_size
            );
            execution_result.durations.resize(execution_data_suspects_size);
            index_c =
                execution_data_suspects_size *
                input_sizes_size;

            indicators::ProgressBar progress_1
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
                indicators::option::PrefixText{ "> Collecting Results: " },
                indicators::option::ShowRemainingTime{true}
            };

            for (index_a = 0;
                index_a < execution_data_suspects_size;
                ++index_a)
            {
                const BenchmarkerSuspectParameters& suspect =
                    execution_data.suspects[index_a];
                execution_result.suspect_names.push_back(suspect.title);
                execution_result.durations[index_a].resize(input_sizes_size);

                for (index_b = 0;
                    index_b < downsample_indices_size;
                    ++index_b)
                {
                    index_d = full_input_sizes[downsample_indices[index_b]];
                    std::optional<std::future<void>> future_opt =
                        UTILITIES.THREAD_POOL.enqueue_task(
                            [&, index_a, index_b, index_d]()
                            {
                                size_t index;
                                for (index = 0;
                                    index < execution_data.warmup_input_count;
                                    ++index)
                                {
                                    BenchmarkerResultData
                                        benchmark_result_execution_data(
                                            index_d
                                        );
                                    execution_data.suspects[index_a]
                                        .on_setup(
                                            benchmark_result_execution_data
                                        );
                                    execution_data.suspects[index_a]
                                        .on_execution(
                                            benchmark_result_execution_data
                                        );
                                }

                                double execution_average = 0.0;
                                for (index = 0;
                                    index < execution_data.input_retry_count;
                                    ++index)
                                {
                                    BenchmarkerResultData
                                        benchmark_result_execution_data(
                                            index_d
                                        );
                                    execution_data.suspects[index_a]
                                        .on_setup(
                                            benchmark_result_execution_data
                                        );
                                    execution_data.suspects[index_a]
                                        .on_execution(
                                            benchmark_result_execution_data
                                        );

                                    execution_average +=
                                        benchmark_result_execution_data
                                        .get_execution_duration_time();
                                    progress_1.set_progress(static_cast<size_t>(
                                        (100.0 * completed_tasks.load()) / index_c));
                                }

                                execution_average = UTILITIES.TIME.convert_nanoseconds(
                                    (execution_average / static_cast<double>(
                                        execution_data.input_retry_count)),
                                    execution_data.y_axis_time_scale_unit
                                );
                                {
                                    std::lock_guard<std::mutex> lock(
                                        result_mutex
                                    );

                                    execution_result.durations[index_a][index_b] =
                                        execution_average;
                                }

                                ++completed_tasks;
                            });

                    if (future_opt.has_value())
                    {
                        scheduled_tasks.emplace_back(
                            index_a,
                            index_b,
                            std::move(future_opt.value())
                        );
                    }
                }
            }

            for (auto& [size, index, future] : scheduled_tasks)
            {
                future.get();
            }

            UTILITIES.CLI_IO.print_with_new_line();

            return true;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_with_timestamp(
                std::string("RuntimePerformanceBenchmarker::_collect_results(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return false;
        }
    }

    bool RuntimePerformanceBenchmarker::_transform_results(
        BenchmarkerExecutionParameters& execution_data,
        BenchmarkerResult& execution_result
    )
    {
        try
        {
            if (!execution_data.is_data_transform_enabled)
            {
                return true;
            }

            return true;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_with_timestamp(
                std::string("RuntimePerformanceBenchmarker::_transform_results(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return false;
        }
    }

    bool RuntimePerformanceBenchmarker::_output_results(
        BenchmarkerExecutionParameters& execution_data,
        BenchmarkerResult& execution_result
    )
    {
        try
        {
            size_t
                size_a = 0,
                size_b =
                execution_data.is_csv_output_enabled +
                execution_data.is_json_output_enabled +
                execution_data.is_jpg_output_enabled +
                execution_data.is_html_output_enabled +
                execution_data.is_svg_output_enabled +
                execution_data.is_gif_output_enabled +
                execution_data.is_txt_output_enabled +
                execution_data.is_eps_output_enabled +
                execution_data.is_tex_output_enabled;

            indicators::ProgressBar progress_1
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
                indicators::option::PrefixText{ "> Output Results: " },
                indicators::option::ShowRemainingTime{true}
            };

            if (execution_data.is_gnuplot_output_enabled)
            {
                std::vector<std::string> line_colors;
                if (execution_data.is_default_line_colors_enabled)
                {
                    line_colors = UTILITIES.DEFAULT_GUI_LINE_COLORS;
                }
                else
                {
                    for (const auto& suspect : execution_data.suspects)
                    {
                        line_colors.push_back(suspect.color_1);
                    }
                }

                std::future<void> execution_data_is_gui_output_future =
                    std::async(std::launch::async, [=]() mutable
                        {
                            matplot::axes_handle matplot_axes_handle;
                            matplot::legend_handle matplot_legend_handle;
                            matplot::grid(true);
                            matplot::gca()->minor_grid(true);
                            matplot::colororder(line_colors);
                            matplot::title(execution_data.title);
                            matplot::gcf()->name(execution_data.title);
                            matplot::xlabel(execution_data.x_title);
                            matplot::ylabel(execution_data.y_title +
                                " In " + UTILITIES.TIME.get_time_unit_full_name(
                                    execution_data.y_axis_time_scale_unit
                                ).data()
                            );

                            switch (execution_data.gnuplot_type)
                            {
                            case BenchmarkerGraph::BOXPLOT:
                                matplot_axes_handle =
                                    matplot::gca();
                                matplot_axes_handle->boxplot(
                                    execution_result.durations
                                );
                                matplot_axes_handle->xticklabels(
                                    execution_result.suspect_names
                                );

                                break;

                            case BenchmarkerGraph::LINEPLOT:
                                matplot::plot(
                                    execution_result.input_sizes,
                                    execution_result.durations
                                );
                                matplot_legend_handle =
                                    matplot::legend(
                                        execution_result.suspect_names
                                    );
                                matplot_legend_handle->location(
                                    UTILITIES.get_benchmark_legend_alignment(
                                        execution_data.legend_alignment
                                    )
                                );
                                matplot_legend_handle->box(false);

                                break;

                            default:
                                matplot::plot(
                                    execution_result.input_sizes,
                                    execution_result.durations
                                );
                                matplot_legend_handle =
                                    matplot::legend(
                                        execution_result.suspect_names
                                    );
                                matplot_legend_handle->location(
                                    UTILITIES.get_benchmark_legend_alignment(
                                        execution_data.legend_alignment
                                    )
                                );
                                matplot_legend_handle->box(false);

                                break;
                            };

                            
                            matplot::show();
                        }
                    );
            }

            std::string current_file_path;
            std::vector<std::future<void>> output_futures;
            std::unordered_map<std::string, bool> matplot_defined_outputs;
            
            if (execution_data.is_file_output_enabled && (
                    execution_data.is_csv_output_enabled ||
                    execution_data.is_json_output_enabled ||
                    execution_data.is_svg_output_enabled ||
                    execution_data.is_eps_output_enabled ||
                    execution_data.is_gif_output_enabled ||
                    execution_data.is_jpg_output_enabled ||
                    execution_data.is_html_output_enabled ||
                    execution_data.is_txt_output_enabled ||
                    execution_data.is_tex_output_enabled
                ))
            {
                current_file_path =
                    BenchmarkerFileSystem::generate_matplot_output_directory_path(
                        execution_data.output_folder_path
                    );
                matplot_defined_outputs["svg"] =
                    execution_data.is_svg_output_enabled;
                matplot_defined_outputs["eps"] =
                    execution_data.is_eps_output_enabled;
                matplot_defined_outputs["gif"] =
                    execution_data.is_gif_output_enabled;
                matplot_defined_outputs["jpg"] =
                    execution_data.is_jpg_output_enabled;
                matplot_defined_outputs["html"] =
                    execution_data.is_html_output_enabled;
                matplot_defined_outputs["txt"] =
                    execution_data.is_txt_output_enabled;
                matplot_defined_outputs["tex"] =
                    execution_data.is_tex_output_enabled;

                for (const auto& [file_name, is_enabled] :
                    matplot_defined_outputs)
                {
                    if (is_enabled)
                    {
                        output_futures.emplace_back(
                            std::async(std::launch::async, [=]()
                                {
                                    matplot::save(
                                        BenchmarkerFileSystem::generate_matplot_output_file(
                                            current_file_path,
                                            file_name
                                        )
                                    );
                                })
                        );
                    }
                }
            }

            if (execution_data.is_file_output_enabled &&
                execution_data.is_csv_output_enabled)
            {
                output_futures.emplace_back(
                    std::async(std::launch::async, [=]() mutable
                        {
                            UTILITIES.TEXT_FILE_IO.set_file_path(
                                BenchmarkerFileSystem::generate_matplot_output_file(
                                    current_file_path, "csv"
                                )
                            );

                            std::string output = "Input Size";
                            for (const std::string suspect_name :
                            execution_result.suspect_names)
                            {
                                output += "," + suspect_name;
                            }
                            output += "\n";

                            size_t index_a, index_b,
                                result_input_size =
                                execution_result.input_sizes.size(),
                                result_suspect_size =
                                execution_result.suspect_names.size(),
                                result_suspect_row_size =
                                result_suspect_size + 1;
                            for (index_a = 0;
                                index_a < result_input_size;
                                ++index_a)
                            {
                                output +=
                                    absl::StrCat(
                                        execution_result.input_sizes[index_a]
                                    );
                                for (index_b = 0;
                                    index_b < result_suspect_size;
                                    ++index_b)
                                {
                                    output += absl::StrCat(
                                        ",",
                                        execution_result.durations[index_b][index_a]
                                    );
                                }
                                output += "\n";
                            }
                            UTILITIES.TEXT_FILE_IO.write_async(output);
                        })
                );
            }

            if (execution_data.is_file_output_enabled &&
                execution_data.is_json_output_enabled)
            {
                output_futures.emplace_back(
                    std::async(std::launch::async, [=]() mutable
                        {
                            UTILITIES.JSON_FILE_IO.set_file_path(
                                BenchmarkerFileSystem::generate_matplot_output_file(
                                    current_file_path, "json"
                                )
                            );

                            std::string output = "{\n";
                            size_t index_a, index_b, size_a, size_b, size_c, size_d;

                            size_a = execution_result.suspect_names.size();
                            size_b = size_a - 1;
                            size_c = execution_result.input_sizes.size();
                            size_d = size_c - 1;
                            for (index_a = 0; index_a < size_a; ++index_a)
                            {
                                output += "\t\t\"" + execution_result.suspect_names[index_a] + "\": [";
                                for (index_b = 0; index_b < size_c; ++index_b)
                                {
                                    output += absl::StrCat(execution_result.durations[index_a][index_b]);

                                    if (index_b < size_d)
                                    {
                                        output += ", ";
                                    }
                                }
                                output += "]";
                                if (index_a < size_b)
                                {
                                    output += ",";
                                }
                                output += "\n";
                            }
                            output += "}\n";

                            UTILITIES.JSON_FILE_IO.write_async(output);
                        })
                );                
            }

            for (auto& output_future : output_futures)
            {
                output_future.get();
                progress_1.set_progress(static_cast<size_t>(
                    (100.0 * ++size_a) / size_b));
            }

            return true;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_with_timestamp(
                std::string("RuntimePerformanceBenchmarker::_output_results(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return false;
        }
    }
}
