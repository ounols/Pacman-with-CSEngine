#include <Component/CameraComponent.h>
#include <Object/SPrefab.h>
#include <Component/RenderComponent.h>
#include <Component/PlayerMovementComponent.h>
#include <Component/MapDataUtil.h>
#include <Component/CameraMovementComponent.h>
#include <Component/LightComponent.h>
#include <Component/GhostMovementComponent.h>
#include <Component/GhostFsmManagerComponent.h>
#include <Component/GhostInBoxState.h>
#include <Component/GhostNoneState.h>
#include <Component/GhostChaseState.h>
#include <Component/PlayerStateComponent.h>
#include <Component/GhostHuntedState.h>
#include <Component/GhostEatenState.h>
#include <Component/AttackItemComponent.h>
#include <Component/GameManagerComponent.h>
#include "Util/Render/SMaterial.h"
#include "FirstDemoScene.h"

using namespace CSE;

FirstDemoScene::FirstDemoScene() = default;


FirstDemoScene::~FirstDemoScene() = default;


void FirstDemoScene::Init() {
    auto camera = new SGameObject("main camera");
    camera->CreateComponent<CameraMovementComponent>();
    auto camera_component = camera->CreateComponent<CameraComponent>();
    camera->GetTransform()->m_position = vec3{ 0, 40.f, 1.5708f};


    auto game_root = new SGameObject("root");
//    SPrefab* plane = SResource::Create<SPrefab>("plane_circle.prefab");
//    auto plane_object = plane->Clone(vec3{0, -1, 0}, game_root);
//    plane_object->GetTransform()->m_rotation = Quaternion::AngleAxis(vec3{1, 0, 0}, 1.5708f);
//    plane_object->GetTransform()->m_scale = vec3{ 100.f, 100.f, 100.f };


//    // Light Object
//    auto light_object = new SGameObject("light");
//    light_object->SetParent(game_root);
//    light_object->GetTransform()->m_position = vec3{ 0.f, 1.f, 0.f };
//    auto light_comp = light_object->CreateComponent<LightComponent>();
//    light_comp->SetLightType(LightComponent::DIRECTIONAL);
//    light_comp->SetDirection(vec4{ 1.0f, 4.0f, 1, 0 });
//    light_comp->m_disableShadow = false;

    // Game Manager
    auto game_manager = new SGameObject("game manager");
    game_manager->SetParent(game_root);
    game_manager->CreateComponent<GameManagerComponent>();

    // Map Mesh
    auto map_prefab = SResource::Create<SPrefab>("map.prefab");
    auto map_mesh = map_prefab->Clone(vec3{ 0, 0, -0.3f }, game_root);
    auto scale_scalar = 0.3f;
    map_mesh->GetTransform()->m_scale = vec3{ scale_scalar, scale_scalar, scale_scalar };
    {
        const auto& children = map_mesh->GetChildren();
        for (const auto& gameObject : children) {
            const auto& component = gameObject->GetComponent<RenderComponent>();
            if(component == nullptr) continue;
            component->SetMaterial(SResource::Create<SMaterial>("File:Material/Map.mat"));
        }
    }
    // Map Util
    auto mapDataUtil = map_mesh->CreateComponent<MapDataUtil>();
    mapDataUtil->InitMapData();

    //Map Object
    auto attack_item_prefab = SResource::Create<SPrefab>("attack_item.prefab");
    auto map_objects = new SGameObject("map objects");
    map_objects->SetParent(game_root);
    const auto& mapDataList = MapDataUtil::GetMapNode();
    const auto& size = MapDataUtil::GetSize();
    for (int y = 0; y < size.y; ++y) {
        for(int x = 0; x < size.x; ++x) {
            const auto& node = mapDataList[x][y];
            if(node == MapDataUtil::REV_ATT) {
                auto attack_item = attack_item_prefab->Clone(MapDataUtil::GetPositionToIndex(ivec2{x, y}), map_objects);
                attack_item->GetTransform()->m_scale = vec3{ scale_scalar, scale_scalar, scale_scalar };
                {
                    const auto& children = attack_item->GetChildren();
                    for (const auto& gameObject : children) {
                        const auto& component = gameObject->GetComponent<RenderComponent>();
                        if(component == nullptr) continue;
                        component->SetMaterial(SResource::Create<SMaterial>("File:Material/Att_Item.mat"));
                    }
                }
                attack_item->CreateComponent<AttackItemComponent>();
            }
        }
    }

    // Player
    auto player_prefab = SResource::Create<SPrefab>("player.prefab");
    auto player = new SGameObject("player object");
    player->SetParent(game_root);
    player->CreateComponent<PlayerMovementComponent>();
    player->CreateComponent<PlayerStateComponent>();
    auto player_mesh = player_prefab->Clone(vec3{0, 0, 0}, player);
    player_mesh->GetTransform()->m_scale = vec3{ scale_scalar, scale_scalar, scale_scalar };
    {
        const auto& children = player_mesh->GetChildren();
        for (const auto& gameObject : children) {
            const auto& component = gameObject->GetComponent<RenderComponent>();
            if(component == nullptr) continue;
            component->SetMaterial(SResource::Create<SMaterial>("File:Material/Player.mat"));
        }
    }
    {
        auto player_light = new SGameObject("player light");
        player_light->SetParent(player);
        player_light->GetTransform()->m_position = vec3 {0, 4, 0};
        auto light_comp = player_light->CreateComponent<LightComponent>();
        light_comp->SetLightType(LightComponent::POINT);
        light_comp->SetColor(vec3{ 500, 500, 0 });
        light_comp->m_disableShadow = true;
        light_comp->SetLightRadius(4);
    }

    //Ghost Manager
    auto ghostMgr = new SGameObject("ghost manager");
    ghostMgr->SetParent(game_root);


    //Ghost
//    auto ghost_prefab = SResource::Create<SPrefab>("ghost.prefab");
//    std::vector<vec3> ghost_color = { vec3{500, 0, 0}, vec3{100, 100, 0}, vec3{0, 0, 300}, vec3{500, 0, 0}};
//    for (int i = 0; i < 4; ++i) {
//        auto ghost = new SGameObject("ghost" + std::to_string(i));
//        ghost->SetParent(ghostMgr);
//        auto ghost_mesh = ghost_prefab->Clone(vec3{0, 0, 0}, ghost);
//        ghost_mesh->GetTransform()->m_scale = vec3{ scale_scalar, scale_scalar, scale_scalar };
//        {
//            const auto& children = ghost_mesh->GetChildren();
//            for (const auto& gameObject : children) {
//                const auto& component = gameObject->GetComponent<RenderComponent>();
//                if(component == nullptr) continue;
//                component->SetMaterial(SResource::Create<SMaterial>("File:Material/Ghost" + std::to_string(i) + ".mat"));
//            }
//        }
//        auto ghost_movement = ghost->CreateComponent<GhostMovementComponent>();
//        if(i == 0) {
//            ghost_movement->SetIndex(ivec2{10, 11});
//        }
//        else {
//            ghost_movement->SetIndex(ivec2{8 + i, 13});
//            auto ghost_inbox = ghost->CreateComponent<GhostInBoxState>();
//            ghost_inbox->SetDelayMilliseconds(i * 1500);
//        }
//        ghost->CreateComponent<GhostNoneState>();
//        ghost->CreateComponent<GhostChaseState>();
//        ghost->CreateComponent<GhostHuntedState>();
//        ghost->CreateComponent<GhostEatenState>();
//        ghost->CreateComponent<GhostFsmManagerComponent>();
//        {
//            auto ghost_light = new SGameObject("ghost" + std::to_string(i) + " light");
//            ghost_light->SetParent(ghost);
//            ghost_light->GetTransform()->m_position = vec3 {0, 4, 0};
//            auto light_comp = ghost_light->CreateComponent<LightComponent>();
//            light_comp->SetLightType(LightComponent::POINT);
//            light_comp->SetColor(ghost_color[i]);
//            light_comp->m_disableShadow = true;
//            light_comp->SetLightRadius(5);
//        }
//    }


// 맵 데이터 디버그 용
//    const auto& mapData = MapDataUtil::GetMapNode();
//    for (int x = 0; x < 20; ++x) {
//        const auto& y_data = mapData[x];
//        for (int y = 0; y < 20; ++y) {
//            const auto data = y_data[y];
//            if(data == MapDataUtil::WALL) {
//                auto position = MapDataUtil::GetPositionToIndex(ivec2{x, y});
//                auto wall_mesh = player_prefab->Clone(position, game_root);
//                wall_mesh->GetTransform()->m_scale = vec3{ scale_scalar, scale_scalar, scale_scalar };
//            }
//        }
//    }


    camera_component->SetTarget(game_root);

}


void FirstDemoScene::Tick(float elapsedTime) {

}


void FirstDemoScene::Destroy() {

}
