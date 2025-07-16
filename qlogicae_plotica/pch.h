#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <matplot/matplot.h>

#include <any>
#include <map>
#include <set>
#include <mutex>
#include <ctime>
#include <regex>
#include <queue>
#include <tuple>
#include <vector>
#include <thread>
#include <future>
#include <string>
#include <memory>
#include <chrono>
#include <limits>
#include <format>
#include <random>
#include <variant>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <typeindex>
#include <optional>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <type_traits>
#include <string_view>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>

#pragma warning(push)
#pragma warning(disable : 4267 4244)
#include <indicators/progress_bar.hpp>
#include <indicators/block_progress_bar.hpp>
#include <indicators/multi_progress.hpp>
#pragma warning(pop)

#include "framework.h"

