//
// Created by ounol on 2021-10-29.
//

#pragma once
#include <Component/SComponent.h>

class PlayerStateComponent : public CSE::SComponent {
public:
    enum State {
        None,
        Hunt,
        Dead,
    };
public:
    PlayerStateComponent();

    ~PlayerStateComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    void SetState(State state) {
        m_startTime = 0;
        m_currentState = state;
    }

    State GetState() const {
        return m_currentState;
    }

private:
    float m_startTime = 0;
    State m_currentState = None;
    float m_refreshTime = 1000.f;
};



