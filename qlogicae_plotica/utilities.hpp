#pragma once

#include "pch.h"

namespace QLogicaePlotica
{
	static constexpr std::string DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER = ".qlogicae";
	static constexpr std::string DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER = "plotica";
	
	struct BenchmarkerSuspectData
	{
		std::string name;
		std::function<void()> callback;
	};

	struct BenchmarkerExecutionData
	{
		std::string name;
		bool is_gui_output_enabled = true;
		bool is_csv_output_enabled = false;
		bool is_json_output_enabled = false;
		uint64_t starting_iteration_count = 1;
		uint64_t incremental_iteration_count = 1;
		uint64_t ending_iteration_count = 1000;
		std::vector<BenchmarkerSuspectData> suspects = {};
	};

	class RuntimePerformanceBenchmarker
	{
	public:
		bool execute(const BenchmarkerExecutionData& data);
		std::future<bool> execute_async(const BenchmarkerExecutionData& data);

		static RuntimePerformanceBenchmarker& get_instance();

	private:
		RuntimePerformanceBenchmarker() = default;
		~RuntimePerformanceBenchmarker() = default;
		RuntimePerformanceBenchmarker(const RuntimePerformanceBenchmarker&) = delete;
		RuntimePerformanceBenchmarker(RuntimePerformanceBenchmarker&&) noexcept = default;
		RuntimePerformanceBenchmarker& operator=(const RuntimePerformanceBenchmarker&) = delete;
		RuntimePerformanceBenchmarker& operator=(RuntimePerformanceBenchmarker&&) noexcept = default;

		mutable std::mutex _mutex;
	};
}



/*

static
execute(settings)

output
- graphs
- filename
	- .qlogicae/.plotica/[date]-[title]-[id]/data.[.csv/.json]
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
