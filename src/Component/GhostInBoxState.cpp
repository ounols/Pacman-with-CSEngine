//
// Created by ounol on 2021-10-28.
//

#include "GhostInBoxState.h"
#include "GhostFsmManagerComponent.h"
#include "GhostNoneState.h"

GhostInBoxState::GhostInBoxState() = default;

GhostInBoxState::~GhostInBoxState() = default;

void GhostInBoxState::Init() {
    GhostFsmStateBaseComponent::Init();
}

void GhostInBoxState::Tick(float elapsedTime) {
    GhostFsmStateBaseComponent::Tick(elapsedTime);
    if(m_startTime <= 0) m_startTime = elapsedTime;
    if(elapsedTime - m_startTime > m_delayMilliseconds) {
        m_fsmManager->ChangeState<GhostNoneState>();
    }
}

void GhostInBoxState::StartState() {
}

void GhostInBoxState::EndState() {

}

void GhostInBoxState::SetDelayMilliseconds(float delayMilliseconds) {
    m_delayMilliseconds = delayMilliseconds;
}
