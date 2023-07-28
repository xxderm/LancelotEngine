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
        void Write(Args... args, const LogLevel& logLevel);
        bool OpenLogFile(const std::string& path);
        void CloseLogFile();
    private:
        ~Log() = default;
        Log() = default;
        std::string GetCurrentTime();
        std::string GetLogLevelStr(const LogLevel& logLevel);
        void AppendLogStr(std::ostringstream& oss) {}
        template<typename T, typename... Args>
        void AppendLogStr(std::ostringstream& oss, const T& arg, Args... args) {
            oss << arg << ' ';
            AppendLogStr(oss, argc...);
        }
    private:
        std::mutex mMtx;
        std::ofstream mFile;
    };
}