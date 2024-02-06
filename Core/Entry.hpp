#pragma once
#include "../Window/IWindow.hpp"
#include "TextGL.hpp"
#include "SDL.h"

namespace LL::Core {

    // TODO: Make separate process handlers and Entry points
    // TODO: Mb make base ProcessHandler

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

    static int LancelotEntry(
            /*std::vector<
                    std::pair<
                                std::shared_ptr<LL::Window::IWindow>,
                                std::shared_ptr<LL::Core::AppStateControl>
                            >
                    > entryStates*/
            std::shared_ptr<LL::Window::IWindow> wnd,
            std::shared_ptr<LL::Core::AppStateControl> app
            ) {
        auto& logger = LL::Core::Log::GetInstance();
        logger.OpenLogFile("log.txt");
        /*
        LL::Window::WindowProperties properties;
        properties.Title = entryState->GetTitle();
        properties.Width = entryState->GetWinSizeW();
        properties.Height = entryState->GetWinSizeH();
        wnd->Initialize(properties);
        wnd->StartState(entryState);
        std::shared_ptr<ProcessHandler> entryHandler = std::make_shared<ProcessHandler>();
        entryHandler->AddWnd(wnd);
        entryHandler->Run();
         */


        /*const auto& firstWin = entryStates.begin()->first;
        const auto& firstApp = entryStates.begin()->second;
        LL::Window::WindowProperties properties;
        properties.Title = firstApp->GetTitle();
        properties.Width = firstApp->GetWinSizeW();
        properties.Height = firstApp->GetWinSizeH();
        firstWin->Initialize(properties);
        firstWin->StartState(firstApp);*/

        std::shared_ptr<ProcessHandler> entryHandler = std::make_shared<ProcessHandler>();
        /*for (auto& state : entryStates) {
            auto& wndCurr = state.first;
            auto& appCurr = state.second;

            std::shared_ptr<Window::WindowProperties> properties = std::make_shared<Window::WindowProperties>();
            properties->Title = appCurr->GetTitle();
            properties->Width = appCurr->GetWinSizeW();
            properties->Height = appCurr->GetWinSizeH();

            wndCurr->Initialize(properties);

            wndCurr->StartState(appCurr);
            entryHandler->AddWnd(wndCurr);
        }*/

        wnd->Initialize(std::make_shared<Window::WindowProperties>());
        wnd->StartState(app);
        entryHandler->AddWnd(wnd);

        entryHandler->Run();
        return 0;
    }
}