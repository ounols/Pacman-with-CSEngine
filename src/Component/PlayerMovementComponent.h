//
// Created by ounol on 2021-10-24.
//

#pragma once

#include <Component/SComponent.h>
#include "DirectionDefine.h"

class PlayerStateComponent;

class PlayerMovementComponent : public CSE::SComponent {
public:
    PlayerMovementComponent();

    ~PlayerMovementComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

private:
    void InputKeyEvent();

    void DetectIndexChanged();

    void UpdateDirection(const CSE::ivec2& index);
    bool HasPath(Direction direction) const;

    void UpdateMovement();

private:
    Direction m_inputDirection = NONE;
    Direction m_direction = NONE;
    std::list<Direction> m_tilePathList;
    CSE::TransformInterface* m_transform = nullptr;
    float m_speed = 0.06f;
    float m_rotateRadian = 0.f;

    CSE::ivec2 m_currentIndex;
    CSE::vec3 m_currentCenterPosition;

    PlayerStateComponent* m_playerState = nullptr;
};