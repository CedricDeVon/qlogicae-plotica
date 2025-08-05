#pragma once

#include "pch.h"

#include "qlogicae_plotica_sandbox.hpp"

template<typename T>
void selection_sort(std::vector<T>& arr) {
    const size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (i != min_idx) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}

template<typename T>
void bubble_sort(std::vector<T>& arr) {
    const size_t n = arr.size();
    bool swapped = true;
    for (size_t i = 0; i < n - 1 && swapped; ++i) {
        swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
    }
}

void insertion_sort(std::vector<double>& items)
{
    const size_t count = items.size();

    for (size_t i = 1; i < count; ++i)
    {
        const double key = items[i];
        size_t j = i;

        while (j > 0 && items[j - 1] > key)
        {
            items[j] = items[j - 1];
            --j;
        }

        items[j] = key;
    }
}

int main()
{
    QLogicaePlotica::RuntimePerformanceBenchmarker& benchmarker =
        QLogicaePlotica::RuntimePerformanceBenchmarker::get_instance();

    benchmarker.execute(
        {
            .title = "Sorting Algorithms",
            .starting_input_count = 1,
            .incremental_input_count = 1,
            .ending_input_count = 10000,
            .input_retry_count = 1,
            .warmup_iterations = 0,
            .maximum_output_count = 10000,
            .y_axis_time_scale_unit =
                QLogicaeCore::TimeScaleUnit::SECONDS,
            .is_csv_output_enabled = true,
            .is_jpg_output_enabled = true,      
            .suspects =
            {
                {
                    .title = "Bubble Sort",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        std::vector<double> items(data.get_input_size());

                        auto& generator = QLogicaeCore::Generator::get_instance();
                        std::generate(items.begin(), items.end(), [&]
                            {
                                return generator.random_double(0.0, 100.0);
                            }
                        );

                        data.set_payload(items);
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        bubble_sort(data.get_payload<std::vector<double>>());
                        data.set_end_execution_time();
                    },
                },
                {
                    .title = "Selection Sort",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        std::vector<double> items(data.get_input_size());

                        auto& generator = QLogicaeCore::Generator::get_instance();
                        std::generate(items.begin(), items.end(), [&]
                            {
                                return generator.random_double(0.0, 100.0);
                            }
                        );

                        data.set_payload(items);
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        selection_sort(data.get_payload<std::vector<double>>());
                        data.set_end_execution_time();
                    }
                },
                {
                    .title = "Insertion Sort",
                    .on_setup = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        std::vector<double> items(data.get_input_size());

                        auto& generator = QLogicaeCore::Generator::get_instance();
                        std::generate(items.begin(), items.end(), [&]
                            {
                                return generator.random_double(0.0, 100.0);
                            }
                        );

                        data.set_payload(items);
                    },
                    .on_execution = [](QLogicaePlotica::BenchmarkerResultData& data)
                    {
                        data.set_start_execution_time();
                        insertion_sort(data.get_payload<std::vector<double>>());
                        data.set_end_execution_time();
                    }
                },
            }            
        }
    );

    return 0;
}
