//
// Created by ounol on 2021-10-24.
//

#include "PlayerMovementComponent.h"
#include "MapDataUtil.h"
#include "PlayerStateComponent.h"

#ifdef WIN32
#include <Windows.h>
#endif

using namespace CSE;
const vec3 ROTATE_VECTOR3 = vec3{0, 1, 0};

COMPONENT_CONSTRUCTOR(PlayerMovementComponent) {
}

PlayerMovementComponent::~PlayerMovementComponent() = default;

void PlayerMovementComponent::Init() {
    m_transform = gameObject->GetTransform();
    m_playerState = gameObject->GetComponent<PlayerStateComponent>();
    m_transform->m_position = MapDataUtil::GetPositionToIndex(ivec2{ 10, 15 });
    m_currentIndex = ivec2{ -1, -1 };
}

void PlayerMovementComponent::Tick(float elapsedTime) {
    if(m_playerState->GetState() == PlayerStateComponent::Dead) {
        m_transform->m_position = MapDataUtil::GetPositionToIndex(ivec2{ 10, 15 });
        m_inputDirection = m_direction = NONE;
        return;
    }
    DetectIndexChanged();
    InputKeyEvent();
    UpdateMovement();
}

void PlayerMovementComponent::InputKeyEvent() {
#ifdef WIN32

    if(GetAsyncKeyState(VkKeyScan('w'))) {
        m_inputDirection = UP;
    }
    if(GetAsyncKeyState(VkKeyScan('a'))) {
        m_inputDirection = LEFT;
    }
    if(GetAsyncKeyState(VkKeyScan('s'))) {
        m_inputDirection = DOWN;
    }
    if(GetAsyncKeyState(VkKeyScan('d'))) {
        m_inputDirection = RIGHT;
    }

    UpdateDirection(m_currentIndex);

#endif
}

void PlayerMovementComponent::Exterminate() {

}

void PlayerMovementComponent::DetectIndexChanged() {
    auto index = MapDataUtil::GetIndexToPosition(m_transform->m_position);
    if(index.x != m_currentIndex.x || index.y != m_currentIndex.y) {

        if(index.y == 13) {
            if(index.x == 0) {
                m_transform->m_position = MapDataUtil::GetPositionToIndex(ivec2{ 19, 13 });
            }
            if(index.x == 20) {
                m_transform->m_position = MapDataUtil::GetPositionToIndex(ivec2{ 1, 13 });
            }
        }

        m_tilePathList = MapDataUtil::GetTilePath(index);
        m_currentCenterPosition = MapDataUtil::GetPositionToIndex(index);
        UpdateDirection(index);
        m_currentIndex = index;
    }
}

void PlayerMovementComponent::UpdateDirection(const ivec2& index) {
    for (const Direction direction : m_tilePathList) {
        if(m_inputDirection == direction) {
            // Left or Right
            if(m_direction % 2 == m_inputDirection % 2) {
                m_direction = direction;
                return;
            }
            if(vec3::Distance(m_transform->m_position, m_currentCenterPosition) > m_speed) return;
            m_direction = direction;
            m_transform->m_position = m_currentCenterPosition;
        }
    }
}

void PlayerMovementComponent::UpdateMovement() {
    vec3 position;
    switch (m_direction) {
        case LEFT:
            position = vec3{-1, 0, 0};
            m_rotateRadian = 0;
            break;
        case RIGHT:
            position = vec3{1, 0, 0};
            m_rotateRadian = Pi;
            break;
        case UP:
            position = vec3{0, 0, -1};
            m_rotateRadian = Pi / 2;
            break;
        case DOWN:
            position = vec3{0, 0, 1};
            m_rotateRadian = -Pi / 2;
            break;
        case NONE:
        default:
            return;
    }
    if(!HasPath(m_direction) &&
            vec3::Distance(m_transform->m_position, m_currentCenterPosition) <= m_speed) {
        m_transform->m_position = m_currentCenterPosition;
        m_direction = NONE;
    }
    else {
        m_transform->m_position += position * m_speed;
    }
    m_transform->m_rotation = Quaternion::AngleAxis(ROTATE_VECTOR3, m_rotateRadian);
}

bool PlayerMovementComponent::HasPath(Direction direction) const {
    for (const Direction direction_element : m_tilePathList) {
        if(direction == direction_element) return true;
    }
    return false;
}
