#include "pch.h"

#include "utilities.hpp"

namespace QLogicaePlotica
{
    Utilities& Utilities::get_instance()
    {
        static Utilities instance;

        return instance;
    }

    std::string Utilities::get_full_name()
    {
        return
            PROJECT_NAME +
            " (" +
            PROJECT_VERSION +
            ")";
    }

    matplot::legend::general_alignment Utilities::get_benchmark_legend_alignment(
        const BenchmarkerLegendAlignment& benchmarker_legend_alignment
    )
    {
        switch (benchmarker_legend_alignment)
        {
            case BenchmarkerLegendAlignment::LEFT:
            {
                return matplot::legend::general_alignment::left;
            }
            case BenchmarkerLegendAlignment::RIGHT:
            {
                return matplot::legend::general_alignment::right;
            }
            case BenchmarkerLegendAlignment::CENTER:
            {
                return matplot::legend::general_alignment::center;
            }
            case BenchmarkerLegendAlignment::TOP:
            {
                return matplot::legend::general_alignment::top;
            }
            case BenchmarkerLegendAlignment::TOP_LEFT:
            {
                return matplot::legend::general_alignment::topleft;
            }
            case BenchmarkerLegendAlignment::TOP_RIGHT:
            {
                return matplot::legend::general_alignment::topright;
            }
            case BenchmarkerLegendAlignment::BOTTOM:
            {
                return matplot::legend::general_alignment::bottom;
            }
            case BenchmarkerLegendAlignment::BOTTOM_LEFT:
            {
                return matplot::legend::general_alignment::bottomleft;
            }
            case BenchmarkerLegendAlignment::BOTTOM_RIGHT:
            {
                return matplot::legend::general_alignment::bottomright;
            }
            default:
            {
                return matplot::legend::general_alignment::topright;
            }
        }
    }
}
