#include "pch.h"

#include "benchmarker_result_data.hpp"

namespace QLogicaePlotica
{
    BenchmarkerResultData::BenchmarkerResultData(
        const size_t& input_size
    ) :
        _input_size(input_size),
        _start_setup_time(0),
        _end_setup_time(0),
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
        _start_execution_time = UTILITIES.TIME.now();
    }

    void BenchmarkerResultData::set_end_execution_time()
    {
        _end_execution_time = UTILITIES.TIME.now();
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
        _start_setup_time = UTILITIES.TIME.now();
    }

    void BenchmarkerResultData::set_end_setup_time()
    {
        _end_setup_time = UTILITIES.TIME.now();
    }

    void BenchmarkerResultData::set_payload(
        const std::any& value
    )
    {
        _payload = value;
    }

}
