//
// Created by ounol on 2021-10-29.
//

#pragma once


#include "GhostFsmStateBaseComponent.h"

class GhostEatenState : public GhostFsmStateBaseComponent {
public:
    GhostEatenState();
    ~GhostEatenState() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    void StartState() override;

    void EndState() override;
private:
    CSE::TransformInterface* m_transform = nullptr;
    CSE::vec3 m_startTilePosition;
    CSE::ivec2 m_startIndex = CSE::ivec2{ 10, 12 };
};



