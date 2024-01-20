#pragma once
#include "App.hpp"

namespace LL::Core {
    class LL_CALL AppStateControl;
    class LL_CALL AppState;
    using AppStateControlPtr = std::shared_ptr<AppStateControl>;
    using AppStatePtr = std::shared_ptr<AppState>;

    class LL_CALL AppState final : std::enable_shared_from_this<AppState> {
    public:
        AppState() = default;
        void ChangeState(const AppStateControlPtr& state);
        void PushScene(const AppStateControlPtr& state);
        void PopScene(const AppStateControlPtr& state);
        void OnUpdate();
        void OnRender();
        void Clean();
    private:
        std::list<AppStateControlPtr> mAppStates;
    };

    class LL_CALL AppStateControl : public App {
    public:
        AppStateControl() = default;
        virtual void OnCreate(AppStatePtr state) {}
        virtual void Clean() {}
        virtual void OnUpdate(AppStatePtr state) {}
        virtual void OnRender(AppStatePtr state) {}
        static void ChangeState(AppStatePtr state, const AppStateControlPtr& control);
        virtual ~AppStateControl() = default;
    };
}