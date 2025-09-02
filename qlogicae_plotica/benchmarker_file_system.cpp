#pragma once

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
            LOGGER.log_async(std::string() + "generate_root_folder(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

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
                DEFAULT_PROJECT_ROOT_OUTPUT_PATH +
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
                DEFAULT_CONFIGURATIONS_FILE_NAME;

            return complete_matplot_output_directory_path;
        }
        catch (const std::exception& exception)
        {
            LOGGER.log_async(std::string() + "generate_configuration_file_path(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

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
                DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER +
                "/" +
                TIME.now(
                    QLogicaeCore::TimeFormat::FULL_DASHED_TIMESTAMP
                );
            complete_matplot_output_directory_path =
                (file_path.empty()) ?
                DEFAULT_PROJECT_ROOT_OUTPUT_PATH +
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
            LOGGER.log_async(std::string() + "generate_matplot_output_directory_path(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

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
                DEFAULT_PROJECT_BENCHMARK_OUTPUT_FILE +
                "." +
                extension_name;
        }
        catch (const std::exception& exception)
        {
            LOGGER.log_async(std::string() + "generate_matplot_output_file(): " + exception.what(), QLogicaeCore::LogLevel::EXCEPTION);

            return "";
        }
    }
}
