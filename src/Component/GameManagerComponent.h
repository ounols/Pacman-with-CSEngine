//
// Created by ounol on 2021-10-29.
//

#pragma once


#include <Component/SComponent.h>

class PlayerStateComponent;

class GameManagerComponent : public CSE::SComponent {
public:
    GameManagerComponent();
    ~GameManagerComponent() override;

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    void GenerateGhosts();

private:
    void ReleaseGhosts();

private:
    PlayerStateComponent* m_playerState = nullptr;
    CSE::SGameObject* m_ghostListObject = nullptr;
    bool m_isRelease = false;
    CSE::SPrefab* m_ghostPrefab = nullptr;
};



