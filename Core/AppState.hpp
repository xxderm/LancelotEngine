#pragma once
#include "App.hpp"

namespace LL::Core {
    class LL_CALL AppStateControl;
    class LL_CALL AppState;
    using AppStateControlPtr = std::shared_ptr<AppStateControl>;
    using AppStatePtr = std::shared_ptr<AppState>;

    class LL_CALL AppState final : public App, std::enable_shared_from_this<AppState> {
    public:
        AppState() = default;
        void ChangeState(const AppStateControlPtr& state);
        void PushScene(const AppStateControlPtr& state);
        void PopScene(const AppStateControlPtr& state);
        void OnUpdate();
        void Clean();
    private:
        std::list<AppStateControlPtr> mAppStates;
    };

    class LL_CALL AppStateControl {
    public:
        AppStateControl() = default;
        virtual void OnCreate(AppStatePtr state) {}
        virtual void Clean() {}
        virtual void OnUpdate(AppStatePtr state) {}
        static void ChangeState(AppStatePtr state, const AppStateControlPtr& control);
        virtual ~AppStateControl() = default;
    };
}