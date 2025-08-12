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
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "RuntimePerformanceBenchmarker::_generate_downsampled_indices(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return {};
        }
    }

    bool RuntimePerformanceBenchmarker::_setup_file_system(
        BenchmarkerExecutionData& execution_data)
    {
        try
        {
            execution_data.output_folder_path = generate_root_folder(
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
                    QLogicaeCore::Constants::FRONT_SLASH +
                    DEFAULT_CONFIGURATIONS_FILE_NAME;

                JSON_FILE_IO.set_file_path(configuration_file_path);
                if (!std::filesystem::exists(configuration_file_path))
                {
                    std::string suspects_json_string = "",
                        suspects_titles_json_string = "\t\t \"titles\": [\n",
                        suspects_color_1s_json_string = "\t\t \"color_1s\": [\n",
                        suspects_color_2s_json_string = "\t\t \"color_2s\": [\n";

                    for (index_a = 0; index_a < size_b; ++index_a)
                    {
                        BenchmarkerSuspectData& benchmarker_suspect_data = execution_data.suspects[index_a];

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
                    suspects_json_string += suspects_titles_json_string + "\t\t],\n" +
                        suspects_color_1s_json_string + "\t\t],\n" +
                        suspects_color_2s_json_string + "\t\t]\n";

                    std::string json_string = std::string() +
                        "{\n" +
                        "\t\"properties\": {\n" +
                        "\t\t\"is_execution_enabled\": " + (execution_data.is_execution_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"title\": \"" + execution_data.title + "\",\n" +
                        "\t\t\"graph\": \"" + convert_benchmarker_graph_from_enum_to_string(execution_data.graph) + "\",\n" +
                        "\t\t\"starting_input_count\": " + absl::StrCat(execution_data.starting_input_count) + ",\n" +
                        "\t\t\"incremental_input_count\": " + absl::StrCat(execution_data.incremental_input_count) + ",\n" +
                        "\t\t\"ending_input_count\": " + absl::StrCat(execution_data.ending_input_count) + ",\n" +
                        "\t\t\"input_retry_count\": " + absl::StrCat(execution_data.input_retry_count) + ",\n" +
                        "\t\t\"warmup_input_count\": " + absl::StrCat(execution_data.warmup_input_count) + ",\n" +
                        "\t\t\"maximum_output_count\": " + absl::StrCat(execution_data.maximum_output_count) + ",\n" +
                        "\t\t\"x_title\": \"" + execution_data.x_title + "\",\n" +
                        "\t\t\"y_title\": \"" + execution_data.y_title + "\",\n" +
                        "\t\t\"legend_alignment\": \"" + convert_benchmark_legend_alignment_from_enum_to_string(execution_data.legend_alignment) + "\",\n" +
                        "\t\t\"y_axis_time_scale_unit\": \"" + TIME.get_time_unit_abbreviation(execution_data.y_axis_time_scale_unit).data() + "\",\n" +
                        "\t\t\"is_file_output_enabled\": " + (execution_data.is_file_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_default_line_color_enabled\": " + (execution_data.is_default_line_color_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_gui_output_enabled\": " + (execution_data.is_gui_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_csv_output_enabled\": " + (execution_data.is_csv_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_json_output_enabled\": " + (execution_data.is_json_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_jpg_output_enabled\": " + (execution_data.is_jpg_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_html_output_enabled\": " + (execution_data.is_html_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_svg_output_enabled\": " + (execution_data.is_svg_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_gif_output_enabled\": " + (execution_data.is_gif_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_txt_output_enabled\": " + (execution_data.is_txt_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_eps_output_enabled\": " + (execution_data.is_eps_output_enabled ? "true" : "false") + ",\n" +
                        "\t\t\"is_tex_output_enabled\": " + (execution_data.is_tex_output_enabled ? "true" : "false") + "\n" +
                        "\t},\n" +
                        "\t\"suspects\": {\n" +
                            suspects_json_string +
                        "\t}\n" +
                        "}\n";

                    JSON_FILE_IO.write_async(json_string);
                }
                else
                {
                    execution_data.is_execution_enabled = JSON_FILE_IO.get_bool({ "properties", "is_execution_enabled" });
                    execution_data.title = JSON_FILE_IO.get_string({ "properties", "title" });
                    execution_data.graph = convert_benchmarker_graph_from_string_to_enum(JSON_FILE_IO.get_string({ "properties", "graph" }));
                    execution_data.starting_input_count = static_cast<size_t>(JSON_FILE_IO.get_double({ "properties", "starting_input_count" }));
                    execution_data.incremental_input_count = static_cast<size_t>(JSON_FILE_IO.get_double({ "properties", "incremental_input_count" }));
                    execution_data.ending_input_count = static_cast<size_t>(JSON_FILE_IO.get_double({ "properties", "ending_input_count" }));
                    execution_data.input_retry_count = static_cast<size_t>(JSON_FILE_IO.get_double({ "properties", "input_retry_count" }));
                    execution_data.warmup_input_count = static_cast<size_t>(JSON_FILE_IO.get_double({ "properties", "warmup_input_count" }));
                    execution_data.maximum_output_count = static_cast<size_t>(JSON_FILE_IO.get_double({ "properties", "maximum_output_count" }));
                    execution_data.x_title = JSON_FILE_IO.get_string({ "properties", "x_title" });
                    execution_data.y_title = JSON_FILE_IO.get_string({ "properties", "y_title" });
                    execution_data.legend_alignment = convert_benchmark_legend_alignment_from_string_to_enum(JSON_FILE_IO.get_string({ "properties", "legend_alignment" }));
                    execution_data.y_axis_time_scale_unit = QLogicaeCore::TimeScaleUnit::NANOSECONDS;
                    execution_data.is_file_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_file_output_enabled" });
                    execution_data.is_default_line_color_enabled = JSON_FILE_IO.get_bool({ "properties", "is_default_line_color_enabled" });
                    execution_data.is_gui_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_gui_output_enabled" });
                    execution_data.is_csv_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_csv_output_enabled" });
                    execution_data.is_json_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_json_output_enabled" });
                    execution_data.is_jpg_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_jpg_output_enabled" });
                    execution_data.is_html_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_html_output_enabled" });
                    execution_data.is_svg_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_svg_output_enabled" });
                    execution_data.is_gif_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_gif_output_enabled" });
                    execution_data.is_txt_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_txt_output_enabled" });
                    execution_data.is_eps_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_eps_output_enabled" });
                    execution_data.is_tex_output_enabled = JSON_FILE_IO.get_bool({ "properties", "is_tex_output_enabled" });

                    std::vector<std::any> titles = JSON_FILE_IO.get_array({ "suspects", "titles" });
                    std::vector<std::any> color_1s = JSON_FILE_IO.get_array({ "suspects", "color_1s" });
                    std::vector<std::any> color_2s = JSON_FILE_IO.get_array({ "suspects", "color_2s" });
                    size_c = titles.size();
                    size_d = color_1s.size();
                    size_e = color_2s.size();

                    if (size_c != size_d || size_c != size_e)
                    {
                        Logger.log_async("RuntimePerformanceBenchmarker::_setup_file_system(): " + std::string("'suspects.titles', 'suspects.color_1s', and 'suspects.color_2s' array properties must be of equal size"), QLogicaeCore::LogLevel::EXCEPTION);

                        return false;
                    }
                    for (index_a = 0; index_a < size_c; ++index_a)
                    {
                        BenchmarkerSuspectData& benchmarker_suspect_data = execution_data.suspects[index_a];

                        benchmarker_suspect_data.title = std::any_cast<std::string>(titles[index_a]);
                        benchmarker_suspect_data.color_1 = std::any_cast<std::string>(color_1s[index_a]);
                        benchmarker_suspect_data.color_2 = std::any_cast<std::string>(color_2s[index_a]);
                    }
                }
            }

            return true;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async("RuntimePerformanceBenchmarker::_setup_file_system(): " + std::string(exception.what()), QLogicaeCore::LogLevel::EXCEPTION);

            return false;
        }
    }

    bool RuntimePerformanceBenchmarker::_setup_benchmark_data(
        BenchmarkerExecutionData& execution_data,
        BenchmarkerResult& execution_result)
    {
        try
        {
            std::mutex result_mutex;
            std::atomic<size_t> completed_tasks = 0;
            size_t index_a, index_b, index_c, index_d,
                execution_data_suspects_size = execution_data.suspects.size();
            std::vector<size_t> full_input_sizes, downsample_indices;
            std::vector<std::tuple<size_t, size_t, std::future<void>>>
                scheduled_tasks;

            for (index_a = execution_data.starting_input_count;
                index_a < execution_data.ending_input_count;
                index_a += execution_data.incremental_input_count)
            {
                full_input_sizes.push_back(index_a);
            }
            downsample_indices = _generate_downsampled_indices(
                full_input_sizes.size(), execution_data.maximum_output_count);
            index_c =
                execution_data_suspects_size * execution_result.input_sizes.size();
            execution_result.input_sizes.reserve(downsample_indices.size());
            for (size_t index : downsample_indices)
            {
                execution_result.input_sizes.push_back(
                    static_cast<double>(full_input_sizes[index]));
            }
            execution_result.suspect_names.reserve(execution_data_suspects_size);
            execution_result.durations.resize(execution_data_suspects_size);

            for (index_a = 0; index_a < execution_data_suspects_size; ++index_a)
            {
                const BenchmarkerSuspectData& suspect =
                    execution_data.suspects[index_a];
                execution_result.suspect_names.push_back(suspect.title);
                execution_result.durations[index_a].resize(execution_result.input_sizes.size());

                for (index_b = 0; index_b < downsample_indices.size(); ++index_b)
                {
                    index_d = full_input_sizes[downsample_indices[index_b]];
                    std::optional<std::future<void>> future_opt =
                        THREAD_POOL.enqueue_task(
                            [&, index_a, index_b, index_d]()
                            {
                                size_t index;
                                for (index = 0;
                                    index < execution_data.warmup_input_count;
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
                                for (index = 0;
                                    index < execution_data.input_retry_count;
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
                                        execution_data.input_retry_count)),
                                    execution_data.y_axis_time_scale_unit
                                );
                                {
                                    std::lock_guard<std::mutex> lock(result_mutex);

                                    execution_result.durations[index_a][index_b] =
                                        execution_average;
                                }

                                ++completed_tasks;
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

            return true;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "RuntimePerformanceBenchmarker::_setup_benchmark_data(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return false;
        }
    }

    bool RuntimePerformanceBenchmarker::_setup_output(
        BenchmarkerExecutionData& execution_data,
        BenchmarkerResult& execution_result)
    {
        try
        {
            if (execution_data.is_gui_output_enabled)
            {
                std::vector<std::string> line_colors;
                if (execution_data.is_default_line_color_enabled)
                {
                    line_colors = DEFAULT_GUI_LINE_COLORS;
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
                            auto ax = matplot::gca();
                            matplot::grid(true);
                            ax->boxplot(execution_result.durations);
                            matplot::colororder(line_colors);
                            matplot::gca()->minor_grid(true);
                            matplot::title(execution_data.title);
                            ax->xticklabels(execution_result.suspect_names);
                            matplot::xlabel(execution_data.x_title);
                            matplot::gcf()->name(execution_data.title);
                            matplot::ylabel(execution_data.y_title +
                                " In " + TIME.get_time_unit_full_name(
                                    execution_data.y_axis_time_scale_unit
                                ).data()
                            );
                            matplot::show();
                        }
                    );
            }

            if (execution_data.is_file_output_enabled)
            {
                std::string current_file_path =
                    generate_matplot_output_directory_path(
                        execution_data.output_folder_path
                    );
                std::vector<std::future<void>> output_futures;
                std::unordered_map<std::string, bool> matplot_defined_outputs;
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
                                        generate_matplot_output_file(
                                            current_file_path,
                                            file_name
                                        )
                                    );
                                })
                        );
                    }
                }

                if (execution_data.is_csv_output_enabled)
                {
                    output_futures.emplace_back(
                        std::async(std::launch::async, [=]() mutable
                            {
                                TEXT_FILE_IO.set_file_path(
                                    generate_matplot_output_file(
                                        current_file_path, "csv"
                                    )
                                );

                                std::string output = "Input Size";
                                for (const std::string suspect_name :
                                execution_result.suspect_names)
                                {
                                    output +=
                                        QLogicaeCore::Constants::COMMA +
                                        suspect_name;
                                }
                                output +=
                                    QLogicaeCore::Constants::END_Of_LINE;

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
                                            QLogicaeCore::Constants::COMMA,
                                            execution_result.durations[index_b][index_a]
                                        );
                                    }
                                    output +=
                                        QLogicaeCore::Constants::END_Of_LINE;
                                }
                                TEXT_FILE_IO.write_async(output);
                            })
                    );
                }

                for (auto& output_future : output_futures)
                {
                    output_future.get();
                }
            }

            return true;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "RuntimePerformanceBenchmarker::_setup_output(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return false;
        }
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute(
        BenchmarkerExecutionData execution_data)
    {
        try
        {
            std::scoped_lock lock(_mutex);
         
            BenchmarkerResult execution_result;
            
            if (!_setup_file_system(execution_data))
            {
                return execution_result;
            }

            if (!_setup_benchmark_data(
                execution_data, execution_result))
            {
                return execution_result;
            }

            if (!_setup_output(
                execution_data, execution_result))
            {
                return execution_result;
            }

            return execution_result;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "RuntimePerformanceBenchmarker::execute(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return BenchmarkerResult();
        }
    }

    std::future<BenchmarkerResult> RuntimePerformanceBenchmarker::execute_async(
        BenchmarkerExecutionData execution_data)
    {
        return std::async(std::launch::async, [this, execution_data]()
            {
                return execute(execution_data);
            }
        );
    }

    std::string generate_root_folder(
        const std::string& file_path,
        const std::string& title)
    {
        try
        {
            std::string complete_folder_path =
                file_path +
                QLogicaeCore::Constants::FRONT_SLASH +
                title;
            if (!std::filesystem::exists(
                complete_folder_path))
            {
                std::filesystem::create_directories(
                    complete_folder_path);
            }

            return complete_folder_path;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "generate_root_folder(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return QLogicaeCore::Constants::EMPTY_STRING;
        }
    }

    std::string generate_configuration_file_path(
            const std::string& file_path,
            const std::string& title)
    {
        try
        {
            std::string complete_matplot_output_directory_path =
                title;
            complete_matplot_output_directory_path =
                (file_path.empty()) ?
                DEFAULT_PROJECT_ROOT_OUTPUT_PATH +
                QLogicaeCore::Constants::FRONT_SLASH +
                complete_matplot_output_directory_path
                :
                file_path +
                QLogicaeCore::Constants::FRONT_SLASH +
                complete_matplot_output_directory_path;
            if (!std::filesystem::exists(
                complete_matplot_output_directory_path))
            {
                std::filesystem::create_directories(
                    complete_matplot_output_directory_path);
            }
            complete_matplot_output_directory_path =
                complete_matplot_output_directory_path +
                QLogicaeCore::Constants::FRONT_SLASH +
                DEFAULT_CONFIGURATIONS_FILE_NAME;

            return complete_matplot_output_directory_path;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "generate_configuration_file_path(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return QLogicaeCore::Constants::EMPTY_STRING;
        }
    }

    

    std::string generate_matplot_output_directory_path(
            const std::string& file_path)
    {
        try
        {
            std::string complete_matplot_output_directory_path =
                TIME.now(
                    QLogicaeCore::TimeFormat::FULL_DASHED_TIMESTAMP
                );
            complete_matplot_output_directory_path =
                (file_path.empty()) ?
                DEFAULT_PROJECT_ROOT_OUTPUT_PATH +
                QLogicaeCore::Constants::FRONT_SLASH +
                complete_matplot_output_directory_path
                :
                file_path +
                QLogicaeCore::Constants::FRONT_SLASH +
                complete_matplot_output_directory_path;
            if (!std::filesystem::exists(
                complete_matplot_output_directory_path))
            {
                std::filesystem::create_directories(
                    complete_matplot_output_directory_path);
            }

            return complete_matplot_output_directory_path;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "generate_matplot_output_directory_path(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return QLogicaeCore::Constants::EMPTY_STRING;
        }
    }

    std::string generate_matplot_output_file(
        const std::string& file_path,
        const std::string& extension_name)
    {
        try
        {
            return file_path +
                QLogicaeCore::Constants::FRONT_SLASH +
                DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER +
                QLogicaeCore::Constants::DOT +
                extension_name;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string() + "generate_matplot_output_file(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return QLogicaeCore::Constants::EMPTY_STRING;
        }
    }

    matplot::legend::general_alignment get_benchmark_legend_alignment(
        const BenchmarkerLegendAlignment& benchmarker_legend_alignment
    )
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

    std::string convert_benchmark_legend_alignment_from_enum_to_string(
        const BenchmarkerLegendAlignment& value
    )
    {
        switch (value)
        {
        case BenchmarkerLegendAlignment::LEFT:
            return "left";

        case BenchmarkerLegendAlignment::RIGHT:
            return "right";

        case BenchmarkerLegendAlignment::CENTER:
            return "center";

        case BenchmarkerLegendAlignment::TOP:
            return "top";

        case BenchmarkerLegendAlignment::TOP_LEFT:
            return "top_left";

        case BenchmarkerLegendAlignment::TOP_RIGHT:
            return "top_right";

        case BenchmarkerLegendAlignment::BOTTOM:
            return "bottom";

        case BenchmarkerLegendAlignment::BOTTOM_LEFT:
            return "bottom_left";

        case BenchmarkerLegendAlignment::BOTTOM_RIGHT:
            return "bottom_right";

        default:
            return "top_right";
        }
    }

    BenchmarkerLegendAlignment
        convert_benchmark_legend_alignment_from_string_to_enum(
            const std::string& value
        )
    {
        std::unordered_map<std::string, BenchmarkerLegendAlignment> values;
        values["left"] = BenchmarkerLegendAlignment::LEFT;
        values["right"] = BenchmarkerLegendAlignment::RIGHT;
        values["center"] = BenchmarkerLegendAlignment::CENTER;
        values["top"] = BenchmarkerLegendAlignment::TOP;
        values["top_left"] = BenchmarkerLegendAlignment::TOP_LEFT;
        values["top_right"] = BenchmarkerLegendAlignment::TOP_RIGHT;
        values["bottom"] = BenchmarkerLegendAlignment::BOTTOM;
        values["bottom_left"] = BenchmarkerLegendAlignment::BOTTOM_LEFT;
        values["bottom_right"] = BenchmarkerLegendAlignment::BOTTOM_RIGHT;

        return values.contains(value) ?
            values[value] :
            BenchmarkerLegendAlignment::TOP_RIGHT;
    }

    std::string convert_benchmarker_graph_from_enum_to_string(
        const BenchmarkerGraph& value
    )
    {
        switch (value)
        {
        case (BenchmarkerGraph::LINE): return "line";
        case (BenchmarkerGraph::BOXPLOT): return "boxplot";
        default: return "boxplot";
        }
    }

    BenchmarkerGraph convert_benchmarker_graph_from_string_to_enum(
        const std::string& value
    )
    {
        std::unordered_map<std::string, BenchmarkerGraph> values;
        values["line"] = BenchmarkerGraph::LINE;
        values["boxplot"] = BenchmarkerGraph::BOXPLOT;

        return values.contains(value) ?
            values[value] :
            BenchmarkerGraph::BOXPLOT;
    }
}
