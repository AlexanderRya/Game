#ifndef GAME_UTIL_HPP
#define GAME_UTIL_HPP

#include <game/Types.hpp>

#include <string>
#include <sstream>
#include <utility>

#if _WIN32
    #include <Windows.h>
    #include <libloaderapi.h>
#elif __linux__
    #include <dlfcn.h>
#endif

namespace game::util {
#if _WIN32
    constexpr inline const wchar_t* vulkan_module = L"vulkan-1.dll";
    HMODULE load_module(LPCWSTR name);
    void (*load_symbol(HMODULE handle, LPCSTR symbol))();
    void close_module(HMODULE handle);
#elif __linux__
    constexpr inline const char* vulkan_module = "libvulkan.so";
    void* load_module(const char* name);
    void (*load_symbol(void* handle, const char* symbol))();
    void close_module(void* handle);
#endif
    template <typename ...Args>
    [[nodiscard]] std::string format(const std::string& str, Args&& ...args) {
        /* Check if argument count and format specifiers match */ {
            u64 start = 0;
            u64 count = 0;

            while ((start = str.find("{}", start)) != std::string::npos) {
                start += 2;
                ++count;
            }

            if (count != sizeof...(args)) {
                throw std::runtime_error("Format specifiers and argument count mismatch");
            }
        }

        auto internal_fmt = [](const std::string& istr, std::stringstream& oss, size_t & start_idx, const auto& var) {
            size_t index = istr.find("{}", start_idx);

            if (index == std::string::npos) {
                return;
            }

            oss << istr.substr(start_idx, index - start_idx) << var;
            start_idx = index + 2;
        };

        std::stringstream oss{};
        size_t start_idx = 0;
        (internal_fmt(str, oss, start_idx, std::forward<Args>(args)), ...);

        return oss << str.substr(start_idx, str.length()), oss.str();
    }

    [[nodiscard]] std::string get_current_timestamp();
} // namespace game::util

#endif //GAME_UTIL_HPP
