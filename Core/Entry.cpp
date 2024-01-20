#include "Entry.hpp"

namespace LL::Core {
    void ProcessHandler::Run() {
        mRunning = true;
        while (mRunning) {
            HandleEvents();
            for (auto &wnd : mWnds) {
                wnd->Clear();
                wnd->Swap();
            }
        }
    }

    void ProcessHandler::Stop() {
        mRunning = false;
    }

    void ProcessHandler::AddWnd(std::shared_ptr<LL::Window::IWindow> wnd) {
        mWnds.push_back(wnd);
    }

    void ProcessHandler::HandleEvents() {

    }
}