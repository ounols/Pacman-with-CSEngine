//
// Created by ounol on 2021-10-28.
//

#pragma once


#include "GhostFsmStateBaseComponent.h"

class GhostInBoxState : public GhostFsmStateBaseComponent {
public:
    GhostInBoxState();
    ~GhostInBoxState() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void StartState() override;

    void EndState() override;

    void SetDelayMilliseconds(float delayMilliseconds);

private:
    float m_startTime = 0;
    float m_delayMilliseconds = 1000.f;
};



