#pragma once
#include "SessionProfiler.hpp"

namespace LL::Core {
    ElapsedTimeLog::ElapsedTimeLog(std::string fnStr) : mFn(fnStr)  {
        mTimer.Start();
    }

    ElapsedTimeLog::~ElapsedTimeLog() {
        mTimer.Stop();
        LL_LOG(LogLevel::INFO, "[Profile] ", mFn, " elapsed in ", mTimer.GetElapsedTime(), "ms");
    }
}