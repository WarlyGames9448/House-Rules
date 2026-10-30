#pragma once

#include "Fuze.h"
#include <fstream>

namespace Fuze {
class FileUtils {
  public:
    static std::filesystem::path GetExecutablePath();
    static std::filesystem::path GetAppAsset(const std::string& file);
    static std::filesystem::path GetSandboxAsset(const std::string& file);
    static std::filesystem::path GetEditorAsset(const std::string& file);

    static bool WriteTextFile(const std::filesystem::path& filepath, const std::string& content);
    static bool WriteBinaryFile(const std::filesystem::path& filepath, const char* data, size_t size);
    static bool WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<char>& data);
};
}
