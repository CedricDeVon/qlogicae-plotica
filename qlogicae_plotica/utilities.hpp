#pragma once

#include "pch.h"

#include "qlogicae_core.hpp"

#include <filesystem>

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

    class Utilities
    {
    public:
        const std::string EXECUTED_FILE_PATH = std::filesystem::current_path()
            .string().c_str();

        const std::string RELATIVE_QLOGICAE_PATH =
            "qlogicae";

        const std::string RELATIVE_DOT_QLOGICAE_PATH =
            ".qlogicae";
        
        const std::string RELATIVE_PLOTICA_PATH =
            "plotica";
        
        const std::string RELATIVE_PLOTICA_CONFIGURATIONS_FILE_PATH =
            "configurations.json";

        const std::string RELATIVE_RESULTS_PATH =
            "results";
        
        const std::string RELATIVE_BENCHMARK_OUTPUT_PATH =
            "data";
        
        const std::string QLOGICAE_DOT_QLOGICAE_PLOTICA_PATH =
            EXECUTED_FILE_PATH +
            "\\" + RELATIVE_QLOGICAE_PATH +
            "\\" + RELATIVE_DOT_QLOGICAE_PATH +
            "\\" + RELATIVE_PLOTICA_PATH;

        QLogicaeCore::ThreadPool& THREAD_POOL =
            QLogicaeCore::ThreadPool::get_instance();
        
        QLogicaeCore::CsvFileIO CSV_FILE_IO;
        
        QLogicaeCore::JsonFileIO JSON_FILE_IO;
        
        QLogicaeCore::TextFileIO TEXT_FILE_IO;
        
        QLogicaeCore::Logger LOGGER;
        
        QLogicaeCore::CliIO& CLI_IO =
            QLogicaeCore::CliIO::get_instance();
        
        QLogicaeCore::Time& TIME =
            QLogicaeCore::Time::get_instance();
        
        QLogicaeCore::Generator& GENERATOR =
            QLogicaeCore::Generator::get_instance();
        
        const std::vector<std::string> DEFAULT_GUI_LINE_COLORS =
        {
            "#FF0000", "#FF8800", "#FFFF00",
            "#00BB00", "#0000FF", "#5500FF",
            "#AA00FF"
        };

        const std::unordered_map<std::string, BenchmarkerLegendAlignment>
            BENCHMARKER_LEGEND_ALIGNMENT_STRINGS =
        {
            { "left", BenchmarkerLegendAlignment::LEFT },
            { "right", BenchmarkerLegendAlignment::RIGHT },
            { "center", BenchmarkerLegendAlignment::CENTER },
            { "top", BenchmarkerLegendAlignment::TOP },
            { "top_left", BenchmarkerLegendAlignment::TOP_LEFT },
            { "top_right", BenchmarkerLegendAlignment::TOP_RIGHT },
            { "bottom", BenchmarkerLegendAlignment::BOTTOM },
            { "bottom_left", BenchmarkerLegendAlignment::BOTTOM_LEFT },
            { "bottom_right", BenchmarkerLegendAlignment::BOTTOM_RIGHT }
        };

        const std::unordered_map<BenchmarkerLegendAlignment, std::string>
            BENCHMARKER_LEGEND_ALIGNMENT_ENUMS =
        {
            { BenchmarkerLegendAlignment::LEFT, "left" },
            { BenchmarkerLegendAlignment::RIGHT, "right" },
            { BenchmarkerLegendAlignment::CENTER, "center" },
            { BenchmarkerLegendAlignment::TOP, "top" },
            { BenchmarkerLegendAlignment::TOP_LEFT, "top_left" },
            { BenchmarkerLegendAlignment::TOP_RIGHT, "top_right" },
            { BenchmarkerLegendAlignment::BOTTOM, "bottom" },
            { BenchmarkerLegendAlignment::BOTTOM_LEFT, "bottom_left" },
            { BenchmarkerLegendAlignment::BOTTOM_RIGHT, "bottom_right" }
        };

        const std::unordered_map<std::string, BenchmarkerGraph>
            BENCHMARKER_GRAPH_STRINGS =
        {
            { "lineplot", BenchmarkerGraph::LINEPLOT },
            { "boxplot", BenchmarkerGraph::BOXPLOT }
        };

        const std::unordered_map<BenchmarkerGraph, std::string>
            BENCHMARKER_GRAPH_ENUMS =
        {
            { BenchmarkerGraph::LINEPLOT, "lineplot" },
            { BenchmarkerGraph::BOXPLOT, "boxplot" }
        };

        matplot::legend::general_alignment get_benchmark_legend_alignment(
            const BenchmarkerLegendAlignment&);

        static Utilities& get_instance();

    protected:
        Utilities() = default;
        ~Utilities() = default;
        Utilities(const Utilities&) = delete;
        Utilities(Utilities&&) noexcept = default;
        Utilities& operator=(const Utilities&) = delete;
        Utilities& operator=(Utilities&&) noexcept = default;
    };

    inline static Utilities& UTILITIES =
        Utilities::get_instance();
}
