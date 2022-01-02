//
// Created by ounol on 2021-10-28.
//

#pragma once
#include <Component/SComponent.h>
#include "GhostFsmStateBaseComponent.h"

class PlayerStateComponent;

class GhostFsmManagerComponent : public CSE::SComponent {
public:
    GhostFsmManagerComponent();

    ~GhostFsmManagerComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    template <class T>
    void ChangeState();

    void UpdateCollisionWithPlayer();

private:
    void DisableAllComponents();
private:
    std::list<GhostFsmStateBaseComponent*> m_components;
    GhostFsmStateBaseComponent* m_currentState = nullptr;

    CSE::TransformInterface* m_transform = nullptr;
    CSE::TransformInterface* m_playerTransform = nullptr;
    PlayerStateComponent* m_playerState = nullptr;
};

template <class T>
void GhostFsmManagerComponent::ChangeState() {
    DisableAllComponents();
    for (const auto& component : m_components) {
        if(component == m_currentState) continue;
        if(dynamic_cast<T*>(component)) {
            if(m_currentState != nullptr)
                m_currentState->EndState();
            m_currentState = component;
            m_currentState->SetIsEnable(true);
            m_currentState->StartState();
        }
    }
}


