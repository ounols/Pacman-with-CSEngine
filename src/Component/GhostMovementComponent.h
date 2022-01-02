//
// Created by ounol on 2021-10-28.
//

#pragma once
#include <Component/SComponent.h>
#include "PlayerMovementComponent.h"
#include "DirectionDefine.h"

class GhostMovementComponent : public CSE::SComponent {
public:
    enum TargetState {
        None,
        Random,
        Player
    };
public:
    GhostMovementComponent();

    ~GhostMovementComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    void DetectIndexChanged();

    void UpdateMovement();

    void UpdatePath();

    void SetPath(CSE::ivec2 target);

    void SetPathRandom();

    void SetTargetState(TargetState state);

    void SetIndex(CSE::ivec2 index);

    void SetSpeed(float speed) {
        m_speed = speed;
    }

private:
    void RefreshPath();

private:
    CSE::TransformInterface* m_transform = nullptr;
    CSE::TransformInterface* m_playerTransform = nullptr;
    float m_speed = 0.06f;

    CSE::ivec2 m_currentIndex;
    CSE::vec3 m_currentCenterPosition;
    CSE::vec3 m_movementDirection;

    std::list<CSE::ivec2> m_pathList;
    TargetState m_targetState = Random;

    CSE::ivec2 m_customIndex;
};



