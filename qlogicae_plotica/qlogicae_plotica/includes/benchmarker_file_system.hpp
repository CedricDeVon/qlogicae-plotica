#pragma once

#include "utilities.hpp"

#include "qlogicae_core/includes/all.hpp"

namespace QLogicaePlotica
{
    class BenchmarkerFileSystem
    {
    public:
        static std::string generate_root_folder(
            const std::string& file_path,
            const std::string& title
        );
        
        static std::string generate_configuration_file_path(
            const std::string& file_path,
            const std::string& title
        );
        
        static std::string generate_matplot_output_file(
            const std::string& file_path,
            const std::string& extension_name
        );
        
        static std::string generate_matplot_output_directory_path(
            const std::string& file_path
        );

    protected:
        BenchmarkerFileSystem() = default;
        
        ~BenchmarkerFileSystem() = default;
        
        BenchmarkerFileSystem(
            const BenchmarkerFileSystem&
        ) = delete;
        
        BenchmarkerFileSystem(
            BenchmarkerFileSystem&&
        ) noexcept = default;
        
        BenchmarkerFileSystem& operator = (
            const BenchmarkerFileSystem&
        ) = delete;
        
        BenchmarkerFileSystem& operator = (
            BenchmarkerFileSystem&&
        ) noexcept = default;
    };
}
