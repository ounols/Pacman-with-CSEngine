//
// Created by ounol on 2021-10-29.
//

#include "AttackItemComponent.h"
#include "PlayerStateComponent.h"

using namespace CSE;

COMPONENT_CONSTRUCTOR(AttackItemComponent) {

}

AttackItemComponent::~AttackItemComponent() = default;

void AttackItemComponent::Init() {
    const auto& player = gameObject->Find("player object");
    m_playerTransform = player->GetTransform();
    m_playerStateComponent = player->GetComponent<PlayerStateComponent>();
    m_transform = gameObject->GetTransform();
}

void AttackItemComponent::Tick(float elapsedTime) {
    m_transform->m_position.y = std::abs(sinf(elapsedTime / 500.f) * 0.5f);
    if(vec3::Distance(m_playerTransform->m_position, m_transform->m_position) <= 1.f) {
        m_playerStateComponent->SetState(PlayerStateComponent::Hunt);
        gameObject->Destroy();
    }
}

void AttackItemComponent::Exterminate() {

}
