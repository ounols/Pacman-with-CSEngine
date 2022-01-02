//
// Created by ounol on 2021-10-28.
//

#include "GhostFsmManagerComponent.h"
#include "GhostInBoxState.h"
#include "GhostNoneState.h"
#include "PlayerStateComponent.h"


using namespace CSE;

COMPONENT_CONSTRUCTOR(GhostFsmManagerComponent) {

}

GhostFsmManagerComponent::~GhostFsmManagerComponent() = default;

void GhostFsmManagerComponent::Init() {
    const auto& components = gameObject->GetComponents();
    for (const auto& component : components) {
        auto converted = dynamic_cast<GhostFsmStateBaseComponent*>(component);
        if(converted) {
            converted->SetFsmManager(this);
            m_components.push_back(converted);
        }
    }
    DisableAllComponents();
    ChangeState<GhostInBoxState>();
    if(m_currentState == nullptr) {
        ChangeState<GhostNoneState>();
    }
    auto player_object = gameObject->Find("player object");
    m_playerTransform = player_object->GetTransform();
    m_playerState = player_object->GetComponent<PlayerStateComponent>();
    m_transform = gameObject->GetTransform();
}

void GhostFsmManagerComponent::Tick(float elapsedTime) {

}

void GhostFsmManagerComponent::Exterminate() {

}

void GhostFsmManagerComponent::DisableAllComponents() {
    for (const auto& component : m_components) {
        component->SetIsEnable(false);
    }
}

void GhostFsmManagerComponent::UpdateCollisionWithPlayer() {
    if(vec3::Distance(m_playerTransform->m_position, m_transform->m_position) < 0.5f) {
        m_playerState->SetState(PlayerStateComponent::Dead);
    }
}
