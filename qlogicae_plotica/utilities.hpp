#pragma once

#include "pch.h"

#include "qlogicae_core.hpp"

namespace QLogicaePlotica
{
    static QLogicaeCore::ThreadPool& THREAD_POOL =
        QLogicaeCore::ThreadPool::get_instance();
    static QLogicaeCore::CsvFileIO CSV_FILE_IO;
    static QLogicaeCore::TextFileIO TEXT_FILE_IO;
    static QLogicaeCore::Time& TIME = QLogicaeCore::Time::get_instance();
    static QLogicaeCore::Generator& GENERATOR = QLogicaeCore::Generator::get_instance();
    static const std::vector<std::string> DEFAULT_GUI_LINE_COLORS =
    {
        "#FF0000", "#FF8800", "#FFFF00",
        "#00BB00", "#0000FF", "#5500FF",
        "#AA00FF"
    };
    // These path variables are strings because matplot++ does not parse std::filesystem::path().string() properly
    static const std::string DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER = ".qlogicae";
    static const std::string DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER = "plotica";
    static const std::string DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER = "data";
    static const std::string DEFAULT_PROJECT_ROOT_OUTPUT_PATH =
        DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER + "/" + DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER;
}
