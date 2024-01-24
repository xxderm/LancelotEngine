#pragma once
#include "pch.hpp"
#include "SessionProfiler.hpp"

namespace LL::Core {

    struct LL_CALL FileData final {
        std::string Path{};
        std::vector<std::string> Text;
    };

    class LL_CALL Resources final {
    public:
        static Resources& GetInstance();
        void LoadFromFile(const std::string& file = "Resources/res.txt");
        std::map<std::string, FileData> GetResources() noexcept;
    private:
        Resources() = default;
        void LoadData(const std::string& file);
        std::string GetParentDir(const std::string& file);
    private:
        std::map<std::string, FileData> mResources;
    };

}