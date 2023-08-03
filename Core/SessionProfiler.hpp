#pragma once
#include "Log.hpp"
#include "Timer.hpp"

#define LL_PROFILE LL::Core::ElapsedTimeLog profile(__func__);

namespace LL::Core {
    class ElapsedTimeLog final {
    public:
        ElapsedTimeLog(std::string fnStr = "");
        ~ElapsedTimeLog();
    private:
        Timer mTimer;
        std::string mFn;
    };
}