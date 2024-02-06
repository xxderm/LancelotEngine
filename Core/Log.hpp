#pragma once
#include "pch.hpp"

#define LL_LOG(logLevel, ...) LL::Core::Log::GetInstance().WriteLog(logLevel, __VA_ARGS__);

namespace LL::Core {
    enum LL_CALL LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERR,
    };

    class LL_CALL Log final {
    public:
        ~Log() { CloseLogFile(); }
        Log() = default;
        static Log& GetInstance();
        template<typename... Args>
        void WriteLog(LogLevel logLevel, Args... args) {
            std::lock_guard<std::mutex> lock(mMtx);

            std::ostringstream oss;
            oss << '[' << GetCurrentTime() << "] "
                << '[' << GetLogLevelStr(logLevel) << "] ";
            this->AppendLogStr(oss, args...);
            oss << '\n';
            if (mFile.is_open())
                mFile << oss.str();
        }
        bool OpenLogFile(const std::string& path);
        void CloseLogFile();
    private:
        static std::string GetCurrentTime();
        static std::string GetLogLevelStr(LogLevel logLevel);
        void AppendLogStr(std::ostringstream& oss) {}
        template<typename T, typename... Args>
        void AppendLogStr(std::ostringstream& oss, const T& arg, Args... args) {
            oss << arg << ' ';
            AppendLogStr(oss, args...);
        }
    private:
        std::mutex mMtx;
        std::ofstream mFile;
    };
}