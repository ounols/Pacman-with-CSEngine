//
// Created by ounol on 2021-10-28.
//

#include "GhostFsmStateBaseComponent.h"
#include "GhostMovementComponent.h"

COMPONENT_CONSTRUCTOR(GhostFsmStateBaseComponent) {

}

GhostFsmStateBaseComponent::~GhostFsmStateBaseComponent() {

}

void GhostFsmStateBaseComponent::Init() {
    m_movementComponent = gameObject->GetComponent<GhostMovementComponent>();
}

void GhostFsmStateBaseComponent::Tick(float elapsedTime) {

}

void GhostFsmStateBaseComponent::Exterminate() {

}

void GhostFsmStateBaseComponent::SetFsmManager(GhostFsmManagerComponent* manager) {
    m_fsmManager = manager;
}
