#pragma once
#include "../Window/IWindow.hpp"

namespace LL::Core {
    class LL_CALL ProcessHandler final {
    public:
        ProcessHandler() = default;
        void Run();
        void Stop();
        void AddWnd(std::shared_ptr<LL::Window::IWindow> wnd);
    private:
        void HandleEvents();
    private:
        std::vector<std::shared_ptr<LL::Window::IWindow>> mWnds;
        bool mRunning = false;
    };

    static int LancelotEntry(std::shared_ptr<LL::Window::IWindow> wnd, std::shared_ptr<LL::Core::AppStateControl> entryState) {
        auto& logger = LL::Core::Log::GetInstance();
        logger.OpenLogFile("log.txt");
        LL::Window::WindowProperties properties;
        properties.Title = entryState->GetTitle();
        properties.Width = entryState->GetWinSizeW();
        properties.Height = entryState->GetWinSizeH();
        wnd->Initialize(properties);
        wnd->StartState(entryState);
        std::shared_ptr<ProcessHandler> entryHandler = std::make_shared<ProcessHandler>();
        entryHandler->AddWnd(wnd);
        entryHandler->Run();
        logger.CloseLogFile();
        return 1;
    }
}