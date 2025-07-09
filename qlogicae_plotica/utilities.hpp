#pragma once

#include "pch.h"

namespace QLogicaePlotica
{
	static constexpr std::string DEFAULT_ROOT_OUTPUT_FOLDER = ".plotica";

	struct BenchmarkerSuspect
	{
		std::string name;
		std::function<void()> callback;
	};

	struct BenchmarkerOptions
	{
		bool is_gui_output_enabled = true;
		bool is_csv_output_enabled = false;
		bool is_json_output_enabled = false;
		uint64_t starting_iteration_count = 1;
		uint64_t incremental_iteration_count = 1;
		uint64_t ending_iteration_count = 1000;
		std::vector<BenchmarkerSuspect> suspects = {};
	};

	class RuntimePerformanceBenchmarker
	{

	};


}



/*

static
execute(settings)

output
- graphs
- filename
	- .plotica/[title]/[id]-[date].[.csv/.json]
- csv
	- iteration_count,...[function_name]
- json
{
	title: string
	timestamp_created: string
	starting_iteration_count: number
	incremental_iteration_count: number
	ending_iteration_count: number
	iterations: number[]
	[function_name]: string[]
}

*/
