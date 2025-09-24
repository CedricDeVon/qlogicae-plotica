#include "pch.h"

#include "benchmarker_file_system.hpp"

namespace QLogicaePlotica
{
    std::string BenchmarkerFileSystem::generate_root_folder(
        const std::string& file_path,
        const std::string& title
    )
    {
        try
        {
            std::string complete_folder_path =
                file_path +
                "/" +
                title;
            if (!std::filesystem::exists(
                complete_folder_path))
            {
                std::filesystem::create_directories(
                    complete_folder_path);
            }

            return complete_folder_path;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_async(
                std::string("BenchmarkerFileSystem::generate_root_folder(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return "";
        }
    }

    std::string BenchmarkerFileSystem::generate_configuration_file_path(
        const std::string& file_path,
        const std::string& title
    )
    {
        try
        {
            std::string complete_matplot_output_directory_path =
                title;
            complete_matplot_output_directory_path =
                (file_path.empty()) ?
                UTILITIES.QLOGICAE_DOT_QLOGICAE_PLOTICA_PATH +
                "/" +
                complete_matplot_output_directory_path
                :
                file_path +
                "/" +
                complete_matplot_output_directory_path;

            if (!std::filesystem::exists(
                complete_matplot_output_directory_path))
            {
                std::filesystem::create_directories(
                    complete_matplot_output_directory_path);
            }
            complete_matplot_output_directory_path =
                complete_matplot_output_directory_path +
                "/" +
                UTILITIES.RELATIVE_PLOTICA_CONFIGURATIONS_FILE_PATH;

            return complete_matplot_output_directory_path;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_async(
                std::string("BenchmarkerFileSystem::generate_configuration_file_path(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return "";
        }
    }

    std::string BenchmarkerFileSystem::
        generate_matplot_output_directory_path(
            const std::string& file_path
        )
    {
        try
        {
            std::string complete_matplot_output_directory_path =
                UTILITIES.RELATIVE_RESULTS_PATH +
                "/" +
                UTILITIES.TIME.now(
                    QLogicaeCore::TimeFormat::FULL_DASHED_TIMESTAMP
                );
            complete_matplot_output_directory_path =
                (file_path.empty()) ?
                UTILITIES.QLOGICAE_DOT_QLOGICAE_PLOTICA_PATH +
                "/" +
                complete_matplot_output_directory_path
                :
                file_path +
                "/" +
                complete_matplot_output_directory_path;

            if (!std::filesystem::exists(
                complete_matplot_output_directory_path))
            {
                std::filesystem::create_directories(
                    complete_matplot_output_directory_path);
            }

            return complete_matplot_output_directory_path;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_async(
                std::string("BenchmarkerFileSystem::generate_matplot_output_directory_path(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return "";
        }
    }

    std::string BenchmarkerFileSystem::generate_matplot_output_file(
        const std::string& file_path,
        const std::string& extension_name
    )
    {
        try
        {
            return file_path +
                "/" +
                UTILITIES.RELATIVE_BENCHMARK_OUTPUT_PATH +
                "." +
                extension_name;
        }
        catch (const std::exception& exception)
        {
            UTILITIES.LOGGER.log_async(
                std::string("BenchmarkerFileSystem::generate_matplot_output_file(): ") +
                exception.what(),
                QLogicaeCore::LogLevel::EXCEPTION
            );

            return "";
        }
    }
}
