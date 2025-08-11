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

    bool RuntimePerformanceBenchmarker::_handle_initial_setup(
        BenchmarkerExecutionData& execution_data,
        const std::string& root_folder_path)
    {
        try
        {
            if (execution_data.is_configuration_parsing_enabled)
            {
                std::string configuration_file_path = root_folder_path +
                    QLogicaeCore::Constants::FRONT_SLASH +
                    DEFAULT_CONFIGURATIONS_FILE_NAME;

                JSON_FILE_IO.set_file_path(configuration_file_path);
                if (!std::filesystem::exists(configuration_file_path))
                {
                    std::string suspects_json_string = "";
                    size_t index,
                        suspects_size = execution_data.suspects.size(),
                        suspects_size_a = suspects_size - 1;

                    suspects_json_string += "\t\t \"titles\": [\n";
                    for (index = 0; index < suspects_size; ++index)
                    {
                        BenchmarkerSuspectData benchmarker_suspect_data = execution_data.suspects[index];
                        suspects_json_string += "\t\t\t\"" + benchmarker_suspect_data.title + "\"";
                        if (index < suspects_size_a)
                        {
                            suspects_json_string += ",";
                        }
                        suspects_json_string += "\n";
                    }
                    suspects_json_string += "\t\t],\n";

                    suspects_json_string += "\t\t \"color_1s\": [\n";
                    for (index = 0; index < suspects_size; ++index)
                    {
                        BenchmarkerSuspectData benchmarker_suspect_data = execution_data.suspects[index];
                        suspects_json_string += "\t\t\t\"" + benchmarker_suspect_data.color_1 + "\"";
                        if (index < suspects_size_a)
                        {
                            suspects_json_string += ",";
                        }
                        suspects_json_string += "\n";
                    }
                    suspects_json_string += "\t\t],\n";

                    suspects_json_string += "\t\t \"color_2s\": [\n";
                    for (index = 0; index < suspects_size; ++index)
                    {
                        BenchmarkerSuspectData benchmarker_suspect_data = execution_data.suspects[index];
                        suspects_json_string += "\t\t\t\"" + benchmarker_suspect_data.color_2 + "\"";
                        if (index < suspects_size_a)
                        {
                            suspects_json_string += ",";
                        }
                        suspects_json_string += "\n";
                    }
                    suspects_json_string += "\t\t]\n";

                    std::string json_string = std::string() +
                        "{\n" +
                        "\t\"properties\": {\n" +
                        "\t\t\"is_execution_enabled\": " + absl::StrCat(execution_data.is_execution_enabled) + ",\n" +
                        "\t\t\"is_configuration_parsing_enabled\": " + absl::StrCat(execution_data.is_configuration_parsing_enabled) + ",\n" +
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
                        "\t\t\"is_default_line_color_enabled\": " + absl::StrCat(execution_data.is_default_line_color_enabled) + ",\n" +
                        "\t\t\"is_gui_output_enabled\": " + absl::StrCat(execution_data.is_gui_output_enabled) + ",\n" +
                        "\t\t\"is_csv_output_enabled\": " + absl::StrCat(execution_data.is_csv_output_enabled) + ",\n" +
                        "\t\t\"is_json_output_enabled\": " + absl::StrCat(execution_data.is_json_output_enabled) + ",\n" +
                        "\t\t\"is_jpg_output_enabled\": " + absl::StrCat(execution_data.is_jpg_output_enabled) + ",\n" +
                        "\t\t\"is_html_output_enabled\": " + absl::StrCat(execution_data.is_html_output_enabled) + ",\n" +
                        "\t\t\"is_svg_output_enabled\": " + absl::StrCat(execution_data.is_svg_output_enabled) + ",\n" +
                        "\t\t\"is_gif_output_enabled\": " + absl::StrCat(execution_data.is_gif_output_enabled) + ",\n" +
                        "\t\t\"is_txt_output_enabled\": " + absl::StrCat(execution_data.is_txt_output_enabled) + ",\n" +
                        "\t\t\"is_eps_output_enabled\": " + absl::StrCat(execution_data.is_eps_output_enabled) + ",\n" +
                        "\t\t\"is_tex_output_enabled\": " + absl::StrCat(execution_data.is_tex_output_enabled) + ",\n" +
                        "\t\t\"output_folder_path\": \"" + execution_data.output_folder_path + "\"\n"
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
                    execution_data.is_configuration_parsing_enabled = JSON_FILE_IO.get_bool({ "properties", "is_configuration_parsing_enabled" });
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
                    execution_data.output_folder_path = JSON_FILE_IO.get_string({ "properties", "output_folder_path" });

                    std::vector<std::any> titles = JSON_FILE_IO.get_array({ "suspects", "titles" });
                    std::vector<std::any> color_1s = JSON_FILE_IO.get_array({ "suspects", "color_1s" });
                    std::vector<std::any> color_2s = JSON_FILE_IO.get_array({ "suspects", "color_2s" });

                    size_t index, titles_size = titles.size(), color_1s_size = color_1s.size(), color_2s_size = color_2s.size();
                    if (titles_size != color_1s_size || titles_size != color_2s_size)
                    {
                        Logger.log_async("'suspects.titles', 'suspects.color_1s', and 'suspects.color_2s' array properties must be of equal size", QLogicaeCore::LogLevel::EXCEPTION);

                        return false;
                    }

                    for (index = 0; index < titles_size; ++index)
                    {
                        execution_data.suspects[index].title = std::any_cast<std::string>(titles[index]);
                        execution_data.suspects[index].color_1 = std::any_cast<std::string>(color_1s[index]);
                        execution_data.suspects[index].color_2 = std::any_cast<std::string>(color_2s[index]);
                    }
                }
            }

            return true;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string(exception.what()), QLogicaeCore::LogLevel::EXCEPTION);

            return false;
        }
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute(
        BenchmarkerExecutionData execution_data)
    {
        try
        {
            std::scoped_lock lock(_mutex);
         
            BenchmarkerResult result;
            std::string root_folder_path = generate_root_folder(
                execution_data.output_folder_path,
                execution_data.title
            );
            bool handle_initial_setup_result = _handle_initial_setup(execution_data, root_folder_path);
            if (!handle_initial_setup_result)
            {
                return BenchmarkerResult();
            }           
            
            return result;
        }
        catch (const std::exception& exception)
        {
            Logger.log_async(std::string(exception.what()), QLogicaeCore::LogLevel::EXCEPTION);

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
        catch (...)
        {
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
        catch (...)
        {
            return QLogicaeCore::Constants::EMPTY_STRING;
        }
    }

    

    std::string generate_matplot_output_directory_path(
            const std::string& file_path,
            const std::string& title)
    {
        try
        {
            std::string complete_matplot_output_directory_path =
                title +
                QLogicaeCore::Constants::FRONT_SLASH +
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
        catch (...)
        {
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
        catch (...)
        {
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
