#pragma once

namespace Fuze {
class FUZE_API FileUtils {
  public:
    static std::filesystem::path GetExecutablePath();
    static std::filesystem::path GetAppAsset(const std::string& file);
    static std::filesystem::path GetSandboxAsset(const std::string& file);
};
}
