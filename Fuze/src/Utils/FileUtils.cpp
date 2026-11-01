#include "fuzepch.h"

#include "Utils/FileUtils.h"

#include <nfd.h>

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

std::filesystem::path FileUtils::GetSandboxAsset(const std::string& file) {
    std::filesystem::path dirPath = GetExecutablePath().parent_path();

    return dirPath / ".." / ".." / "Sandbox" / "assets" / file;
}

std::filesystem::path FileUtils::GetEditorAsset(const std::string& file) {
    std::filesystem::path dirPath = GetExecutablePath().parent_path();

    return dirPath / ".." / ".." / "FuzeEditor" / "assets" / file;
}

bool FileUtils::WriteTextFile(const std::filesystem::path& filepath, const std::string& content) {
    if (filepath.has_parent_path() && !std::filesystem::exists(filepath.parent_path())) {
        std::filesystem::create_directories(filepath.parent_path());
    }

    std::ofstream file(filepath, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    file << content;
    return file.good();
}

bool FileUtils::WriteBinaryFile(const std::filesystem::path& filepath, const char* data, size_t size) {
    if (filepath.has_parent_path() && !std::filesystem::exists(filepath.parent_path())) {
        std::filesystem::create_directories(filepath.parent_path());
    }

    std::ofstream file(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    file.write(data, size);
    return file.good();
}

bool FileUtils::WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<char>& data) {
    return WriteBinaryFile(filepath, data.data(), data.size());
}

std::optional<std::filesystem::path> FileUtils::OpenFile(const std::string& filter, const std::filesystem::path& initialDirectory) {
    nfdu8char_t* outPath = nullptr;

    nfdu8filteritem_t filters[] = {
        {"Fuze Scene", filter.c_str()}
    };

    const std::string directory = initialDirectory.empty() ? std::string {} : initialDirectory.string();

    const nfdresult_t result = NFD_OpenDialogU8(&outPath, filters, 1, directory.empty() ? nullptr : directory.c_str());

    if (result == NFD_CANCEL) return std::nullopt;

    if (result == NFD_ERROR) {
        FUZE_CORE_ASSERT(0, "Error to Open File!");
        return std::nullopt;
    }

    std::filesystem::path path = std::filesystem::path((const char8_t*)outPath);
    NFD_FreePathU8(outPath);

    return path;
}

std::optional<std::filesystem::path> FileUtils::SaveFile(const std::string& filter, const std::string& defaultName,
                                                         const std::filesystem::path& initialDirectory) {
    nfdu8char_t* outPath = nullptr;

    nfdu8filteritem_t filters[] = {
        {"Fuze Scene", filter.c_str()}
    };

    const std::string directory = initialDirectory.empty() ? std::string {} : initialDirectory.string();

    const nfdresult_t result = NFD_SaveDialogU8(&outPath, filters, 1, directory.empty() ? nullptr : directory.c_str(), defaultName.c_str());

    if (result == NFD_CANCEL) return std::nullopt;

    if (result == NFD_ERROR) {
        FUZE_CORE_ASSERT(0, "Error to Save File!");
        return std::nullopt;
    }

    std::filesystem::path path = std::filesystem::path((const char8_t*)outPath);

    NFD_FreePathU8(outPath);

    return path;
}
}
