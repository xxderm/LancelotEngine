#include "AppState.hpp"

namespace LL::Core {
    void AppState::ChangeState(const AppStateControlPtr &state) {
        if (!mAppStates.empty()) {
            mAppStates.back()->Clean();
            mAppStates.back()->OnCreate(this->weak_from_this().lock());
        }
        mAppStates.push_back(state);
        mAppStates.back()->OnCreate(this->weak_from_this().lock());
    }

    void AppState::PushScene(const AppStateControlPtr &state) {
        mAppStates.push_back(state);
        mAppStates.back()->OnCreate(this->weak_from_this().lock());
    }

    void AppState::PopScene(const AppStateControlPtr &state) {
        if (!mAppStates.empty()) {
            mAppStates.back()->Clean();
            mAppStates.pop_back();
        }
    }

    void AppState::OnUpdate() {
        mAppStates.back()->OnUpdate(this->weak_from_this().lock());
    }

    void AppState::Clean() {
        while (!mAppStates.empty()) {
            mAppStates.back()->Clean();
            mAppStates.pop_back();
        }
    }

    void AppState::OnRender() {
        mAppStates.back()->OnRender(this->weak_from_this().lock());
    }

    void AppStateControl::ChangeState(AppStatePtr state, const AppStateControlPtr &control) {
        state->ChangeState(control);
    }
}