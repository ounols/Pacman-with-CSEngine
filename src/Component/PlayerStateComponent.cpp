//
// Created by ounol on 2021-10-29.
//

#include "PlayerStateComponent.h"

COMPONENT_CONSTRUCTOR(PlayerStateComponent) {

}

PlayerStateComponent::~PlayerStateComponent() = default;

void PlayerStateComponent::Init() {
}

void PlayerStateComponent::Tick(float elapsedTime) {
    if(m_startTime <= 0) m_startTime = elapsedTime;
    if(m_currentState != None && elapsedTime - m_startTime > m_refreshTime) {
        m_currentState = None;
    }
}

void PlayerStateComponent::Exterminate() {

}