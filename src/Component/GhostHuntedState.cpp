//
// Created by ounol on 2021-10-29.
//

#include "GhostHuntedState.h"
#include "PlayerStateComponent.h"
#include "GhostFsmManagerComponent.h"
#include "GhostMovementComponent.h"
#include "GhostNoneState.h"
#include "GhostEatenState.h"
#include <Util/Render/SMaterial.h>
#include <Component/RenderComponent.h>

using namespace CSE;

GhostHuntedState::GhostHuntedState() = default;

GhostHuntedState::~GhostHuntedState() = default;

void GhostHuntedState::Init() {
    GhostFsmStateBaseComponent::Init();
    m_playerTransform = gameObject->Find("player object")->GetTransform();
    m_transform = gameObject->GetTransform();

    const auto& renderComponent = GetRenderComponent(*gameObject); // 렌더 컴포넌트를 찾는 함수
    m_material = renderComponent->GetMaterial();
}

CSE::RenderComponent* GhostHuntedState::GetRenderComponent(const CSE::SGameObject& object) const {
    const auto& children = object.GetChildren();
    for (const auto& gameObject : children) {
        const auto& component = gameObject->GetComponent<RenderComponent>();
        if(component == nullptr) {
            const auto& result = GetRenderComponent(*gameObject);
            if(result != nullptr) return result;
        }
        return component;
    }
    return nullptr;
}

void GhostHuntedState::Tick(float elapsedTime) {
    GhostFsmStateBaseComponent::Tick(elapsedTime);
    if(m_startTime <= 0) m_startTime = elapsedTime;
    m_movementComponent->UpdatePath(); // 길찾기로 도착하면 다시 갱신하는 함수
    if(vec3::Distance(m_playerTransform->m_position, m_transform->m_position) <= 1.f) {
        m_fsmManager->ChangeState<GhostEatenState>(); // 잡히면 박스로 돌아가는 상태로 전환
        return;
    }
    if(elapsedTime - m_startTime > m_durationSeconds * 1000.f) {
        m_fsmManager->ChangeState<GhostNoneState>(); // 일정 시간이 지나면 원래 상태로 전환
    }
}

void GhostHuntedState::Exterminate() {
    GhostFsmStateBaseComponent::Exterminate();
}

void GhostHuntedState::StartState() {
    m_movementComponent->SetSpeed(0.03f); // 이속 감소
    m_movementComponent->SetPathRandom(); // 랜덤하게 이동하도록 수정
    m_material->SetVec3("vec3.albedo", vec3{ 0.01f, 0.01f, 0.01f }); // 고스트를 회색으로 전환
}

void GhostHuntedState::EndState() {
    m_movementComponent->SetSpeed(0.06f); // 이속 복구
    m_material->SetVec3("vec3.albedo", vec3{ 1, 1, 1 }); // 고스트 색상 복구
    m_startTime = 0;
}
