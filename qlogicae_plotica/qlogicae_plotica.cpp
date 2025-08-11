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
        try
        {
            std::scoped_lock lock(_mutex);
            
            BenchmarkerResult result;
            
            return result;
        }
        catch (...)
        {
            return BenchmarkerResult();
        }
    }

    BenchmarkerResult RuntimePerformanceBenchmarker::execute_a(
        const BenchmarkerExecutionData& execution_data)
    {
        try
        {
            std::scoped_lock lock(_mutex);
            
            BenchmarkerResult result;
            
            return result;
        }
        catch (...)
        {
            return BenchmarkerResult();
        }
    }

    std::future<BenchmarkerResult> RuntimePerformanceBenchmarker::execute_async(
        const BenchmarkerExecutionData& execution_data)
    {
        return std::async(std::launch::async, [this, execution_data]()
            {
                return execute(execution_data);
            }
        );
    }

    std::future<BenchmarkerResult> RuntimePerformanceBenchmarker::execute_a_async(
        const BenchmarkerExecutionData& execution_data)
    {
        return std::async(std::launch::async, [this, execution_data]()
            {
                return execute_a(execution_data);
            }
        );
    }

    std::string RuntimePerformanceBenchmarker::
        _generate_matplot_output_directory_path(
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

    std::string RuntimePerformanceBenchmarker::_generate_matplot_output_file(
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
