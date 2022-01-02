//
// Created by ounol on 2021-10-28.
//

#pragma once
#include <Component/SComponent.h>

class GhostFsmManagerComponent;
class GhostMovementComponent;

class GhostFsmStateBaseComponent : public CSE::SComponent {
public:
    GhostFsmStateBaseComponent();

    ~GhostFsmStateBaseComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    virtual void StartState() = 0;
    virtual void EndState() = 0;

    void SetFsmManager(GhostFsmManagerComponent* manager);

protected:
    GhostFsmManagerComponent* m_fsmManager = nullptr;
    GhostMovementComponent* m_movementComponent = nullptr;
};



