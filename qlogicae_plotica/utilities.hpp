#pragma once

#include "pch.h"

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

    enum class BenchmarkerGraph : uint8_t
    {
        LINEPLOT,
        BOXPLOT
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

    std::string convert_benchmarker_graph_from_enum_to_string(
        const BenchmarkerGraph& value
    );

    BenchmarkerGraph convert_benchmarker_graph_from_string_to_enum(
        const std::string& value
    );
    // These path variables are strings because matplot++ does not parse std::filesystem::path().string() properly
    static const std::string DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER =
        ".qlogicae";
    static const std::string DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER =
        "plotica";
    static const std::string DEFAULT_CONFIGURATIONS_FILE_NAME =
        "configurations.json";

    static const std::string DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER =
        "results";
    static const std::string DEFAULT_PROJECT_BENCHMARK_OUTPUT_FILE =
        "data";
    static const std::string DEFAULT_PROJECT_ROOT_OUTPUT_PATH =
        DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER + "/" +
        DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER;
    static QLogicaeCore::ThreadPool& THREAD_POOL =
        QLogicaeCore::ThreadPool::get_instance();
    static QLogicaeCore::CsvFileIO CSV_FILE_IO;
    static QLogicaeCore::JsonFileIO JSON_FILE_IO;
    static QLogicaeCore::TextFileIO TEXT_FILE_IO;
    static QLogicaeCore::Logger LOGGER;
    static QLogicaeCore::CliIO& CLI_IO =
        QLogicaeCore::CliIO::get_instance();
    static QLogicaeCore::Time& TIME =
        QLogicaeCore::Time::get_instance();
    static QLogicaeCore::Generator& GENERATOR =
        QLogicaeCore::Generator::get_instance();
    static const std::vector<std::string> DEFAULT_GUI_LINE_COLORS =
    {
        "#FF0000", "#FF8800", "#FFFF00",
        "#00BB00", "#0000FF", "#5500FF",
        "#AA00FF"
    };
    static QLogicaeCore::Statistician& STATISTICIAN =
        QLogicaeCore::Statistician::get_instance();
}
