//
// Created by ounol on 2021-10-29.
//

#include <iostream>
#include "GhostEatenState.h"
#include "MapDataUtil.h"
#include "GhostMovementComponent.h"
#include "GhostFsmManagerComponent.h"
#include "GhostNoneState.h"

using namespace CSE;

GhostEatenState::GhostEatenState() = default;

GhostEatenState::~GhostEatenState() = default;

void GhostEatenState::Init() {
    GhostFsmStateBaseComponent::Init();
    m_transform = gameObject->GetTransform();
    m_startTilePosition = MapDataUtil::GetPositionToIndex(m_startIndex);
}

void GhostEatenState::Tick(float elapsedTime) {
    GhostFsmStateBaseComponent::Tick(elapsedTime);
    if(vec3::Distance(m_startTilePosition, m_transform->m_position) <= 0.5f) {
        m_fsmManager->ChangeState<GhostNoneState>(); // 박스로 되돌아가면 원래 상태로 전환
    }
}

void GhostEatenState::Exterminate() {
    GhostFsmStateBaseComponent::Exterminate();
}

void GhostEatenState::StartState() {
    m_movementComponent->SetSpeed(0.2f); // 빠르게 복귀하도록 이속을 증가
    m_movementComponent->SetTargetState(GhostMovementComponent::None); // 특정 위치로 이동하도록 타겟 설정
    m_movementComponent->SetPath(m_startIndex); // 박스 위치로 이동 위치 지정
}

void GhostEatenState::EndState() {
    m_movementComponent->SetSpeed(0.06f); // 이속 복구
}
