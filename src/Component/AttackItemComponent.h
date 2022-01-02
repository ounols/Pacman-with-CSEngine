//
// Created by ounol on 2021-10-29.
//

#pragma once


#include <Component/SComponent.h>

class PlayerStateComponent;

class AttackItemComponent : public CSE::SComponent {
public:
    AttackItemComponent();
    ~AttackItemComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;
private:
    CSE::TransformInterface* m_transform = nullptr;
    CSE::TransformInterface* m_playerTransform = nullptr;
    PlayerStateComponent* m_playerStateComponent = nullptr;
};



