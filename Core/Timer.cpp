#include "Timer.hpp"

namespace LL::Core {
    void Timer::Start() {
        mStartTime = std::chrono::high_resolution_clock::now();
    }

    void Timer::Stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - mStartTime);
        mElapsedTime = duration.count();
    }

    Timer::Timer() {
        this->Start();
    }

    Timer::~Timer() noexcept {
        this->Stop();
    }

    long long Timer::GetElapsedTime() {
        return mElapsedTime;
    }
}