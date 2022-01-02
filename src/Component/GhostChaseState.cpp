//
// Created by ounol on 2021-10-28.
//

#include "GhostChaseState.h"
#include "GhostMovementComponent.h"
#include "GhostNoneState.h"
#include "GhostFsmManagerComponent.h"
#include "PlayerStateComponent.h"
#include "GhostHuntedState.h"
#include "GhostEatenState.h"

using namespace CSE;

std::random_device randomChaseDevice;
std::mt19937 generatorChase(randomChaseDevice());

GhostChaseState::GhostChaseState() = default;

GhostChaseState::~GhostChaseState() = default;

void GhostChaseState::Init() {
    GhostFsmStateBaseComponent::Init();
    auto player_object = gameObject->Find("player object");
    m_playerTransform = player_object->GetTransform();
    m_playerState = player_object->GetComponent<PlayerStateComponent>();
    m_transform = gameObject->GetTransform();
    m_random = std::uniform_int_distribution<int>(1000, 20000);
}

void GhostChaseState::Tick(float elapsedTime) {
    GhostFsmStateBaseComponent::Tick(elapsedTime);
    if(m_playerState->GetState() == PlayerStateComponent::Hunt) {
        m_fsmManager->ChangeState<GhostHuntedState>(); // 플레이어가 파워 필을 먹으면 상태 전환
        return;
    }
    if(m_startTime <= 0) m_startTime = elapsedTime;
    m_movementComponent->UpdatePath(); // 길찾기로 도착하면 다시 갱신하는 함수
    m_fsmManager->UpdateCollisionWithPlayer(); // 플레이어와의 충돌 처리
    if(vec3::Distance(m_playerTransform->m_position, m_transform->m_position) > m_chaseRadius ||
            elapsedTime - m_startTime > m_durationSeconds * 1000.f) {
        // 일정 거리를 벗어나거나 일정 시간이 지나면 원래 상태로 전환
        m_fsmManager->ChangeState<GhostNoneState>();
    }
}

void GhostChaseState::Exterminate() {
    GhostFsmStateBaseComponent::Exterminate();
}

void GhostChaseState::StartState() {
    m_movementComponent->SetTargetState(GhostMovementComponent::Player); // 플레이어로 가도록 경로 지정
    m_durationSeconds = m_random(generatorChase) / 1000.f;
}

void GhostChaseState::EndState() {
    m_startTime = 0;
}
