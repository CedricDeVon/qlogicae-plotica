#pragma once

#include "framework.h"

#include "qlogicae_core/includes/all.hpp"

#include <matplot/matplot.h>

#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>

#pragma warning(push)
#pragma warning(disable : 4267 4244)
#include <indicators/progress_bar.hpp>
#include <indicators/block_progress_bar.hpp>
#include <indicators/multi_progress.hpp>
#pragma warning(pop)

#include <any>
#include <variant>
#include <optional>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <compare>
#include <source_location>

#include <cstddef>
#include <cstdint>
#include <climits>
#include <limits>
#include <bit>
#include <bitset>

#include <string>
#include <string_view>
#include <charconv>
#include <format>
#include <regex>
#include <cctype>
#include <cwctype>

#include <array>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <span>
#include <tuple>
#include <stack>
#include <queue>
#include <iterator>
#include <ranges>

#include <memory>
#include <memory_resource>
#include <new>

#include <thread>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>
#include <stop_token>
#include <semaphore>
#include <latch>
#include <barrier>

#include <chrono>
#include <ratio>
#include <random>
#include <numbers>

#include <fstream>
#include <filesystem>

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <cstdio>
#include <cstdlib>

#include <stdexcept>
#include <cassert>
#include <cerrno>
#include <csignal>
#include <cmath>
#include <complex>
#include <valarray>

#include <algorithm>
#include <functional>
#include <execution>
#include <locale>

#include <version>

#include <concepts>
#include <coroutine>

#include <cfenv>
#include <cfloat>

#include <cstdarg>
#include <csetjmp>
#include <cstring>
#include <ctime>

#include <windows.h>

namespace QLogicaePlotica
{

}
