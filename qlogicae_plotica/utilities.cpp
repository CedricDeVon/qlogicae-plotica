#pragma once

#include "pch.h"

#include "utilities.hpp"

namespace QLogicaePlotica
{
    std::unordered_map<std::string, BenchmarkerLegendAlignment>
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

    std::unordered_map<std::string, BenchmarkerGraph>
        BENCHMARKER_GRAPH_STRINGS =
    {
        { "lineplot", BenchmarkerGraph::LINEPLOT },
        { "boxplot", BenchmarkerGraph::BOXPLOT }
    };

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
        return BENCHMARKER_LEGEND_ALIGNMENT_STRINGS.contains(value) ?
            BENCHMARKER_LEGEND_ALIGNMENT_STRINGS[value] :
            BenchmarkerLegendAlignment::TOP_RIGHT;
    }

    std::string convert_benchmarker_graph_from_enum_to_string(
        const BenchmarkerGraph& value
    )
    {
        switch (value)
        {
        case (BenchmarkerGraph::LINEPLOT):
            return "lineplot";

        case (BenchmarkerGraph::BOXPLOT):
            return "boxplot";

        default:
            return "lineplot";
        }
    }

    BenchmarkerGraph convert_benchmarker_graph_from_string_to_enum(
        const std::string& value
    )
    {
        return BENCHMARKER_GRAPH_STRINGS.contains(value) ?
            BENCHMARKER_GRAPH_STRINGS[value] :
            BenchmarkerGraph::LINEPLOT;
    }
}
