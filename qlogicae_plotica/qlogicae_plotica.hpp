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
        CENTER,
        NONE
    };

    matplot::legend::general_alignment get_benchmark_legend_alignment(
        const BenchmarkerLegendAlignment&);

    std::string convert_benchmark_legend_alignment_from_enum_to_string(
        const BenchmarkerLegendAlignment& value
    );

    BenchmarkerLegendAlignment
        convert_benchmark_legend_alignment_from_string_to_enum(
            const std::string& value
    );

    enum class BenchmarkerGraph : uint8_t
    {
        LINE,
        BOXPLOT,
        NONE
    };

    std::string convert_benchmarker_graph_from_enum_to_string(
        const BenchmarkerGraph& value
    );

    BenchmarkerGraph convert_benchmarker_graph_from_string_to_enum(
        const std::string& value
    );

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
        std::string color_1;
        std::string color_2;
        std::function<void(BenchmarkerResultData&)> on_setup
            = [](BenchmarkerResultData& data) {};
        std::function<void(BenchmarkerResultData&)> on_execution
            = [](BenchmarkerResultData& data) {};
    };

    struct BenchmarkerExecutionData
    {
        bool is_execution_enabled = true;
        bool is_configuration_parsing_enabled = true;
        std::string title = "Title";
        BenchmarkerGraph graph = BenchmarkerGraph::BOXPLOT;
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
            QLogicaeCore::TimeScaleUnit::NANOSECONDS;
        bool is_file_output_enabled = true;
        bool is_default_line_color_enabled = true;
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
        std::string output_folder_path = DEFAULT_PROJECT_ROOT_OUTPUT_PATH;
        std::vector<BenchmarkerSuspectData> suspects;
    };

    static BenchmarkerExecutionData default_result;
    
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
        BenchmarkerResult execute(BenchmarkerExecutionData data);
        std::future<BenchmarkerResult> execute_async(
            BenchmarkerExecutionData data);
        
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
            BenchmarkerExecutionData& execution_data);
        bool _setup_benchmark_data(
            BenchmarkerExecutionData& execution_data,
            BenchmarkerResult& execution_result);
        bool _setup_output(
            BenchmarkerExecutionData& execution_data,
            BenchmarkerResult& execution_result);
        std::vector<size_t> _generate_downsampled_indices(
            const size_t& total_points, const size_t& maximum_points) const;
    };

    std::string generate_root_folder(
        const std::string& file_path,
        const std::string& title);
    
    std::string generate_configuration_file_path(
        const std::string& file_path,
        const std::string& title);
    
    std::string generate_matplot_output_file(
        const std::string& file_path,
        const std::string& extension_name);
    
    std::string generate_matplot_output_directory_path(
        const std::string& file_path);
}


/*


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
            const bool execution_data_is_default_line_color_enabled =
                execution_data.is_default_line_color_enabled;
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
            std::unordered_map<std::string, bool> matplot_defined_outputs;
            std::vector<std::string> line_colors;
            if (execution_data_is_default_line_color_enabled)
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
                                TEXT_FILE_IO.set_file_path(
                                    _generate_matplot_output_file(
                                        current_file_path, "csv"
                                    )
                                );

                                std::string output = "Input Size";
                                for (const std::string suspect_name :
                                result.suspect_names)
                                {
                                    output += QLogicaeCore::Constants::COMMA + suspect_name;
                                }
                                output += QLogicaeCore::Constants::END_Of_LINE;

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
                                            QLogicaeCore::Constants::COMMA,
                                            result.durations[index_b][index_a]
                                        );
                                    }
                                    output += QLogicaeCore::Constants::END_Of_LINE;
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

            progress.mark_as_completed();

            return result;
        }
        catch (...)
        {
            return result;
        }
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute_a(
        const BenchmarkerExecutionData& execution_data)
    {
        std::scoped_lock lock(_mutex);

        BenchmarkerResult result;
        try
        {
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
            const bool execution_data_is_default_line_color_enabled =
                execution_data.is_default_line_color_enabled;
            const size_t execution_data_title_size =
                execution_data.title.size();
            const size_t execution_data_suspects_size =
                execution_data.suspects.size();
            const size_t execution_data_input_count =
                execution_data.input_count;
            const size_t execution_data_warmup_iterations =
                execution_data.warmup_iterations;
            const size_t execution_data_input_retry_count =
                execution_data.input_retry_count;
            const size_t execution_data_maximum_output_count =
                execution_data.maximum_output_count;
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
                execution_data_input_count < 1 ||
                !execution_data_suspects_size ||
                execution_data_input_retry_count < 0)
            {
                result.success = false;

                return result;
            }
            std::vector<std::string> line_colors;
            if (execution_data_is_default_line_color_enabled)
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
            std::atomic<size_t> completed_tasks = 0;
            size_t index_a, index_b, index_c, index_d,
                full_input_sizes_size,
                downsample_indices_size,
                result_input_sizes_size;
            std::vector<size_t> full_input_sizes, downsample_indices;
            std::vector<std::future<void>> output_futures;
            std::vector<std::tuple<size_t, size_t, std::future<void>>>
                scheduled_tasks;
            std::unordered_map<std::string, bool> matplot_defined_outputs;

            for (index_a = 0;
                index_a < execution_data_input_retry_count;
                ++index_a)
            {
                full_input_sizes.push_back(execution_data_input_count);
            }
            full_input_sizes_size = full_input_sizes.size();

            downsample_indices = _generate_downsampled_indices(
                full_input_sizes_size, execution_data_maximum_output_count);
            downsample_indices_size = downsample_indices.size();

            result.input_sizes.reserve(downsample_indices_size);
            for (size_t index : downsample_indices)
            {
                result.input_sizes.push_back(
                    static_cast<double>(full_input_sizes[index]));
            }
            result_input_sizes_size = result.input_sizes.size();
            result.suspect_names.reserve(execution_data_suspects_size);
            result.durations.resize(execution_data_suspects_size);
            index_c = execution_data_suspects_size * result_input_sizes_size;

            std::mutex result_mutex;
            for (index_a = 0; index_a < execution_data_suspects_size; ++index_a)
            {
                const BenchmarkerSuspectData& suspect =
                    execution_data.suspects[index_a];
                result.suspect_names.push_back(suspect.title);
                result.durations[index_a].resize(result_input_sizes_size);

                for (index_b = 0; index_b < downsample_indices_size; ++index_b)
                {
                    index_d = full_input_sizes[downsample_indices[index_b]];
                    std::optional<std::future<void>> future_opt =
                        THREAD_POOL.enqueue_task(
                            [&, index_a, index_b, index_d]()
                            {
                                size_t index;
                                double execution_average = 0.0;

                                for (index = 0;
                                    index < execution_data_warmup_iterations;
                                    ++index)
                                {
                                    BenchmarkerResultData benchmark_result_execution_data(
                                        index_d
                                    );
                                    execution_data.suspects[index_a].on_setup(
                                        benchmark_result_execution_data);
                                    execution_data.suspects[index_a].on_execution(
                                        benchmark_result_execution_data);
                                }

                                for (index = 0;
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
                            auto ax = matplot::gca();
                            matplot::grid(true);
                            ax->boxplot(result.durations);
                            matplot::colororder(line_colors);
                            matplot::gca()->minor_grid(true);
                            matplot::title(execution_data_title);
                            ax->xticklabels(result.suspect_names);
                            matplot::xlabel(execution_data_x_title);
                            matplot::gcf()->name(execution_data_title);
                            matplot::ylabel(execution_data_y_title +
                                " In " + TIME.get_time_unit_full_name(
                                    execution_data_y_axis_time_scale_unit
                                ).data()
                            );
                            matplot::show();
                        }
                    );
            }

            if (execution_data_is_file_output_enabled)
            {
                std::string current_file_path =
                    _generate_matplot_output_directory_path(
                        execution_data_output_folder_path,
                        execution_data_title
                    );
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
                                TEXT_FILE_IO.set_file_path(
                                    _generate_matplot_output_file(
                                        current_file_path, "csv"
                                    )
                                );

                                std::string output = "Input Size";
                                for (const std::string suspect_name :
                                result.suspect_names)
                                {
                                    output +=
                                        QLogicaeCore::Constants::COMMA +
                                        suspect_name;
                                }
                                output +=
                                    QLogicaeCore::Constants::END_Of_LINE;

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
                                        absl::StrCat(
                                            result.input_sizes[index_a]
                                        );
                                    for (index_b = 0;
                                        index_b < result_suspect_size;
                                        ++index_b)
                                    {
                                        output += absl::StrCat(
                                            QLogicaeCore::Constants::COMMA,
                                            result.durations[index_b][index_a]
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

    std::future<BenchmarkerResult> RuntimePerformanceBenchmarker::execute_a_async(
        const BenchmarkerExecutionData& execution_data)
    {
        return std::async(std::launch::async, [this, execution_data]()
            {
                return execute_a(execution_data);
            });
    }

*/