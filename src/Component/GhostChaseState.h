//
// Created by ounol on 2021-10-28.
//

#pragma once


#include <random>
#include "GhostFsmStateBaseComponent.h"

class PlayerStateComponent;

class GhostChaseState : public GhostFsmStateBaseComponent {
public:
    GhostChaseState();
    ~GhostChaseState() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    void StartState() override;

    void EndState() override;
private:
    CSE::TransformInterface* m_playerTransform = nullptr;
    CSE::TransformInterface* m_transform = nullptr;
    PlayerStateComponent* m_playerState = nullptr;
    float m_chaseRadius = 10.f;
    float m_startTime = 0;
    float m_durationSeconds = 1.f;
    std::uniform_int_distribution<int> m_random;
};



