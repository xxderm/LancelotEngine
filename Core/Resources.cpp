#include "Resources.hpp"

namespace LL::Core {

    LL::Core::Resources &LL::Core::Resources::GetInstance() {
        static Resources instance;
        return instance;
    }

    void Resources::LoadFromFile(const std::string& file) {
        LoadData(file);
    }

    void Resources::LoadData(const std::string &file) {
        std::ifstream input;
        input.open(file);
        if (!input.is_open()) {
            LL_LOG(Core::ERR, "Cant open resources file: ", file);
            input.close();
            return;
        }
        std::string index, path;
        while (input >> index >> path) {
            LL_LOG(Core::INFO, "[Resources] Process: index", index, " with path: ", path);
            FileData fileData;
            auto currentDir = GetParentDir(file);
            currentDir += path;

            std::ifstream toInput;
            toInput.open(currentDir);
            if (!toInput.is_open()) {
                LL_LOG(Core::ERR, "Cant open resources file: ", currentDir);
                toInput.close();
                continue;
            }
            std::string line;
            while (getline(toInput, line))
                fileData.Text.push_back(line);
            fileData.Path = currentDir;
            this->mResources.insert( { index, fileData } );
        }
        input.close();
    }

    std::map<std::string, FileData> Resources::GetResources() noexcept {
        return mResources;
    }

    std::string Resources::GetParentDir(const std::string& file) {
        if (file.empty()) {
            LL_LOG(Core::ERR, "Cant get parent of file: ", file);
            return "";
        }
        std::string result = file;
        for (int i = file.size() - 1; i >= 0; --i) {
            if (file.at(i) == '/' or file.at(i) == '\\') {
                break;
            }
            result.erase(result.begin() + i);
        }
        return result;
    }

}
