#pragma once

#include "framework.h"

#include "qlogicae_core.hpp"

#include <any>
#include <mutex>
#include <future>
#include <vector>
#include <string>
#include <cstdint>
#include <windows.h>
#include <filesystem>

#include <matplot/matplot.h>

#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>

#pragma warning(push)
#pragma warning(disable : 4267 4244)
#include <indicators/progress_bar.hpp>
#include <indicators/block_progress_bar.hpp>
#include <indicators/multi_progress.hpp>
#pragma warning(pop)
