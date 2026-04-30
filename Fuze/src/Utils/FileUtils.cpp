#include "fuzepch.h"

#include "Utils/FileUtils.h"

namespace Fuze {
std::filesystem::path FileUtils::GetExecutablePath() {
#if defined(FUZE_PLATFORM_WINDOWS)
    wchar_t path[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, path, MAX_PATH);
    return std::filesystem::path(path);

#elif defined(FUZE_PLATFORM_MACOS)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        return std::filesystem::path(path);
    }
    return std::filesystem::path();

#elif defined(FUZE_PLATFORM_LINUX)
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count != -1) {
        return std::filesystem::path(std::string(result, count));
    }
    return std::filesystem::path();

#else
    #error "OS not supported for get_executable_path"
    return std::filesystem::path();
#endif
}

std::filesystem::path FileUtils::GetAppAsset(const std::string& file) {
    std::filesystem::path dirPath = GetExecutablePath().parent_path();

    return dirPath / ".." / ".." / "Fuze" / "assets" / file;
}

std::filesystem::path FileUtils::GetSandboxAsset(const std::string& file){
    std::filesystem::path dirPath = GetExecutablePath().parent_path();

    return dirPath / ".." / ".." / "Sandbox" / "assets" / file;
}
}
