//
// Created by ounol on 2021-10-29.
//

#include <Object/SResource.h>
#include <Object/SPrefab.h>
#include <Util/Render/SMaterial.h>
#include <Component/RenderComponent.h>
#include <Component/LightComponent.h>
#include "GameManagerComponent.h"
#include "PlayerStateComponent.h"
#include "GhostMovementComponent.h"
#include "GhostInBoxState.h"
#include "GhostNoneState.h"
#include "GhostHuntedState.h"
#include "GhostChaseState.h"
#include "GhostEatenState.h"
#include "GhostFsmManagerComponent.h"

using namespace CSE;

COMPONENT_CONSTRUCTOR(GameManagerComponent) {

}

GameManagerComponent::~GameManagerComponent() = default;

void GameManagerComponent::Init() {
    m_playerState = gameObject->Find("player object")->GetComponent<PlayerStateComponent>();
    m_ghostListObject = gameObject->Find("ghost manager");
    m_ghostPrefab = SResource::Create<SPrefab>("ghost.prefab");
    GenerateGhosts();
}

void GameManagerComponent::Tick(float elapsedTime) {
    if(!m_isRelease && m_playerState->GetState() == PlayerStateComponent::Dead) {
        ReleaseGhosts();
        GenerateGhosts();
        m_isRelease = true;
    }
    if(m_isRelease && m_playerState->GetState() != PlayerStateComponent::Dead) {
        m_isRelease = false;
    }
}

void GameManagerComponent::Exterminate() {

}

void GameManagerComponent::ReleaseGhosts() {
    const auto& children = m_ghostListObject->GetChildren();
    auto iter = children.begin();

    while (iter != children.end()) {
        const auto& child = *iter;
        ++iter;
        child->Destroy();
    }
}

void GameManagerComponent::GenerateGhosts() {
    const auto& children = m_ghostListObject->GetChildren();
    if(!children.empty()) return;

    //Ghost
    std::vector<vec3> ghost_color = { vec3{500, 0, 0}, vec3{100, 100, 0}, vec3{0, 0, 300}, vec3{500, 0, 0}};
    for (int i = 0; i < 4; ++i) {
        auto ghost = new SGameObject("ghost" + std::to_string(i));
        ghost->SetParent(m_ghostListObject);
        auto ghost_mesh = m_ghostPrefab->Clone(vec3{0, 0, 0}, ghost);
        auto scale_scalar = 0.3f;
        ghost_mesh->GetTransform()->m_scale = vec3{ scale_scalar, scale_scalar, scale_scalar };
        {
            const auto& children = ghost_mesh->GetChildren();
            for (const auto& gameObject : children) {
                const auto& component = gameObject->GetComponent<RenderComponent>();
                if(component == nullptr) continue;
                component->SetMaterial(SResource::Create<SMaterial>("File:Material/Ghost" + std::to_string(i) + ".mat"));
            }
        }
        auto ghost_movement = ghost->CreateComponent<GhostMovementComponent>();
        if(i == 0) {
            ghost_movement->SetIndex(ivec2{10, 11});
        }
        else {
            ghost_movement->SetIndex(ivec2{8 + i, 13});
            auto ghost_inbox = ghost->CreateComponent<GhostInBoxState>();
            ghost_inbox->SetDelayMilliseconds(i * 1500);
        }
        ghost->CreateComponent<GhostNoneState>();
        ghost->CreateComponent<GhostChaseState>();
        ghost->CreateComponent<GhostHuntedState>();
        ghost->CreateComponent<GhostEatenState>();
        ghost->CreateComponent<GhostFsmManagerComponent>();
        {
            auto ghost_light = new SGameObject("ghost" + std::to_string(i) + " light");
            ghost_light->SetParent(ghost);
            ghost_light->GetTransform()->m_position = vec3 {0, 4, 0};
            auto light_comp = ghost_light->CreateComponent<LightComponent>();
            light_comp->SetLightType(LightComponent::POINT);
            light_comp->SetColor(ghost_color[i]);
            light_comp->SetShadow(false);
            light_comp->SetLightRadius(5);
        }
    }
}
