#pragma once

#include "pch.h"

#include "time.hpp"
#include "generator.hpp"
#include "thread_pool.hpp"

namespace QLogicaePlotica
{
    static const std::vector<std::string> DEFAULT_GUI_LINE_COLORS =
    {
        "#FF0000", "#FF8800", "#FFFF00",
        "#00BB00", "#0000FF", "#5500FF",
        "#AA00FF"
    };
    static const std::string DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER = ".qlogicae";
    static const std::string DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER = "plotica";
    static const std::string DEFAULT_PROJECT_BENCHMARK_OUTPUT_FOLDER = "data";
    static const std::string DEFAULT_PROJECT_ROOT_OUTPUT_PATH =
        DEFAULT_COMPANY_ROOT_OUTPUT_FOLDER + "/" + DEFAULT_PROJECT_ROOT_OUTPUT_FOLDER;
    static QLogicaeCore::ThreadPool& THREAD_POOL =
        QLogicaeCore::ThreadPool::get_instance();
    static QLogicaeCore::Time& TIME = QLogicaeCore::Time::instance();
    static QLogicaeCore::Generator& GENERATOR = QLogicaeCore::Generator::instance();
}
