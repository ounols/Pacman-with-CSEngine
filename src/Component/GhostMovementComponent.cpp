//
// Created by ounol on 2021-10-28.
//

#include <random>
#include <iostream>
#include "GhostMovementComponent.h"
#include "MapDataUtil.h"

using namespace CSE;

std::random_device randomDevice;
std::mt19937 generator(randomDevice());

COMPONENT_CONSTRUCTOR(GhostMovementComponent) {

}

GhostMovementComponent::~GhostMovementComponent() = default;

void GhostMovementComponent::Init() {
    m_transform = gameObject->GetTransform();

    const auto& playerObject = gameObject->Find("player object");
    m_playerTransform = playerObject->GetTransform();
}

void GhostMovementComponent::Tick(float elapsedTime) {
    DetectIndexChanged();
    UpdateMovement();
}

void GhostMovementComponent::Exterminate() {

}

void GhostMovementComponent::DetectIndexChanged() {
    if(vec3::Distance(m_transform->m_position, m_currentCenterPosition) > m_speed) return;
    if(m_pathList.empty()) return;

    auto index = MapDataUtil::GetIndexToPosition(m_transform->m_position);

    if(index.x != m_currentIndex.x || index.y != m_currentIndex.y) {
        m_pathList.pop_front();
        if(m_pathList.empty()) return;
        auto currentPosition = MapDataUtil::GetPositionToIndex(index);
        auto nextPosition = MapDataUtil::GetPositionToIndex(m_pathList.front());
        m_movementDirection = (nextPosition - currentPosition).Normalized();
        if(std::isnan(m_movementDirection.x)) {
            m_currentIndex = MapDataUtil::GetIndexToPosition(m_transform->m_position);
            RefreshPath();
            return;
        }
        m_transform->m_position = currentPosition;
        m_currentCenterPosition = nextPosition;
        m_currentIndex = index;
    }
}

void GhostMovementComponent::UpdateMovement() {
    if(m_pathList.empty()) {
        return;
    }
    m_transform->m_position += m_movementDirection * m_speed;
}

void GhostMovementComponent::SetPath(CSE::ivec2 target) {
    m_customIndex = target;
    m_pathList.clear();
    if(m_currentIndex.x < 0) m_currentIndex = MapDataUtil::GetIndexToPosition(m_transform->m_position);
    m_pathList = MapDataUtil::PathFinding(m_currentIndex, target);
    m_currentIndex = {-1, -1};
    DetectIndexChanged();
}

void GhostMovementComponent::SetPathRandom() {
    m_targetState = Random;
    const auto& size = MapDataUtil::GetSize();
    std::uniform_int_distribution<int> x(1, size.x - 1);
    std::uniform_int_distribution<int> y(1, size.y - 1);
    bool hasIndex = false;
    ivec2 index = ivec2();
    do {
        index.Set(x(generator), y(generator));
        if(std::abs(index.x - m_currentIndex.x) <= 2 || std::abs(index.y - m_currentIndex.y) <= 2) continue;
        if(std::abs(index.x - m_currentIndex.x) > 10 || std::abs(index.y - m_currentIndex.y) > 10) continue;
        auto node = MapDataUtil::GetNode(index);
        hasIndex = node < 8;
    } while (!hasIndex);

    SetPath(index);
}

void GhostMovementComponent::SetTargetState(TargetState state) {
    m_targetState = state;
    RefreshPath();
}

void GhostMovementComponent::RefreshPath() {
    if(m_targetState == Player) {
        SetPath(MapDataUtil::GetIndexToPosition(m_playerTransform->m_position));
    }
    else if(m_targetState == Random) {
        SetPathRandom();
    }
    else {
        SetPath(m_customIndex);
    }
}

void GhostMovementComponent::SetIndex(CSE::ivec2 index) {
    if(m_transform == nullptr) {
        m_transform = gameObject->GetTransform();
    }
    m_currentIndex = index;
    m_currentCenterPosition = m_transform->m_position = MapDataUtil::GetPositionToIndex(index);
}

void GhostMovementComponent::UpdatePath() {
    if(m_pathList.empty()) {
        RefreshPath();
    }
}
