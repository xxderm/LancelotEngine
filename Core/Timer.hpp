#pragma once
#include "pch.hpp"
#include "../impexp.hpp"

namespace LL::Core {
    class LL_CALL Timer final {
    public:
        Timer();
        void Start();
        void Stop();
        long long GetElapsedTime();
        ~Timer();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime{};
        long long mElapsedTime;
    };
}