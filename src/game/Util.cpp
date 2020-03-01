#include <game/Util.hpp>

#include <chrono>
#include <string>

namespace snake::util {
#if _WIN32
    #include <libloaderapi.h>
    HMODULE load_module(LPCWSTR name) {
        return LoadLibraryW(name);
    }
    void (*load_symbol(HMODULE handle, LPCSTR symbol))() {
        return reinterpret_cast<void(*)()>(GetProcAddress(handle, symbol));
    }
    void close_module(HMODULE handle) {
        FreeLibrary(handle);
    }
#elif __linux__
#include <dlfcn.h>
    void* load_module(const char* name) {
        return dlopen(name, RTLD_LAZY | RTLD_LOCAL);
    }
    void (*load_symbol(void* handle, const char* symbol))() {
        return reinterpret_cast<void(*)()>(dlsym(handle, symbol));
    }
    void close_module(void* handle) {
        dlclose(handle);
    }
#endif

    std::string get_current_timestamp() {
        namespace ch = std::chrono;

        std::time_t time = ch::duration_cast<ch::seconds>(ch::system_clock::now().time_since_epoch()).count();

        std::string buf(128, '\0');
        buf.resize(std::strftime(buf.data(), buf.size(), "%Y-%m-%d %X", std::localtime(&time)));

        return buf;
    }
} // namespace snake::util