//
// Created by ounol on 2021-10-28.
//

#include "GhostNoneState.h"
#include "GhostMovementComponent.h"
#include "GhostFsmManagerComponent.h"
#include "GhostChaseState.h"
#include "PlayerStateComponent.h"
#include "GhostHuntedState.h"

using namespace CSE;

GhostNoneState::GhostNoneState() = default;

GhostNoneState::~GhostNoneState() = default;

void GhostNoneState::Init() {
    GhostFsmStateBaseComponent::Init();
    auto player_object = gameObject->Find("player object");
    m_playerTransform = player_object->GetTransform();
    m_playerState = player_object->GetComponent<PlayerStateComponent>();
    m_transform = gameObject->GetTransform();
}

void GhostNoneState::Tick(float elapsedTime) {
    GhostFsmStateBaseComponent::Tick(elapsedTime);
    if(m_playerState->GetState() == PlayerStateComponent::Hunt) {
        m_fsmManager->ChangeState<GhostHuntedState>(); // 플레이어가 파워 필을 먹으면 상태 전환
        return;
    }
    if(m_startTime <= 0) m_startTime = elapsedTime;
    m_movementComponent->UpdatePath(); // 길찾기로 도착하면 다시 갱신하는 함수
    m_fsmManager->UpdateCollisionWithPlayer(); // 플레이어와의 충돌 처리
    if(elapsedTime - m_startTime < m_safeChangeMilliseconds) return;
    if(vec3::Distance(m_playerTransform->m_position, m_transform->m_position) < m_chaseRadius) {
        m_fsmManager->ChangeState<GhostChaseState>(); // 일정 거리에 들어오면 쫓는 상태로 전환
    }
}

void GhostNoneState::Exterminate() {
    GhostFsmStateBaseComponent::Exterminate();
}

void GhostNoneState::StartState() {
    m_movementComponent->SetPathRandom(); // 랜덤하게 이동하도록 선언
}

void GhostNoneState::EndState() {
    m_startTime = 0;
}
