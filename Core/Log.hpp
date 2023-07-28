#pragma once
#include "../impexp.hpp"
#include "pch.hpp"

namespace LL::Core {
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class LL_CALL Log {
    public:
        static Log& GetInstance() { static Log instance; return instance; }
        template<typename... Args>
        void Write(LogLevel logLevel, Args... args) {
            std::lock_guard<std::mutex> lock(mMtx);

            std::ostringstream oss;
            oss << '[' << GetCurrentTime() << "] "
                << '[' << GetLogLevelStr(logLevel) << "] ";
            this->AppendLogStr(oss, args...);
            if (mFile.is_open()) {
                mFile << oss.str();
                mFile.flush();
            }
        }
        bool OpenLogFile(const std::string& path);
        void CloseLogFile();
    private:
        ~Log() = default;
        Log() = default;
        std::string GetCurrentTime();
        std::string GetLogLevelStr(LogLevel logLevel);
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