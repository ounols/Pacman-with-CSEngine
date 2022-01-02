//
// Created by ounol on 2021-10-29.
//

#pragma once


#include "GhostFsmStateBaseComponent.h"

namespace CSE {
    class SMaterial;
    class RenderComponent;
}


class GhostHuntedState : public GhostFsmStateBaseComponent {
public:
    GhostHuntedState();
    ~GhostHuntedState() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    void StartState() override;

    void EndState() override;

private:
    CSE::RenderComponent* GetRenderComponent(const CSE::SGameObject& object) const;

private:
    float m_startTime = 0;
    float m_durationSeconds = 10.f;
    CSE::TransformInterface* m_playerTransform = nullptr;
    CSE::TransformInterface* m_transform = nullptr;
    CSE::SMaterial* m_material = nullptr;
};



