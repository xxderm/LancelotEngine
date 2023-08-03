#include "Log.hpp"

namespace LL::Core {
    Log& Log::GetInstance() {
        static Log instance;
        return instance;
    }

    std::string Log::GetCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto cTime = std::chrono::system_clock::to_time_t(now);
        std::string tStr = std::ctime(&cTime);
        tStr.pop_back();
        return tStr;
    }

    std::string Log::GetLogLevelStr(LogLevel logLevel) {
        std::string levelStr{};
        switch (logLevel) {
            case LogLevel::DEBUG:
                levelStr = "DEBUG";
                break;
            case LogLevel::INFO:
                levelStr = "INFO";
                break;
            case LogLevel::WARNING:
                levelStr = "WARNING";
                break;
            case LogLevel::ERR:
                levelStr = "ERROR";
                break;
            default:
                levelStr = "UNKNOWN";
                break;
        }
        return levelStr;
    }

    bool Log::OpenLogFile(const std::string& path) {
        std::lock_guard<std::mutex> lock(mMtx);
        mFile.open(path, std::ofstream::out | std::ofstream::trunc);
        return mFile.is_open();
    }

    void Log::CloseLogFile() {
        std::lock_guard<std::mutex> lock(mMtx);
        if (mFile.is_open())
            mFile.close();
    }
}