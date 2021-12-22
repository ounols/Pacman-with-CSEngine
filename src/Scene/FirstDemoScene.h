#pragma once
#include "Manager/Base/Scene.h"
#include "Object/SGameObject.h"
#include "Util/Render/MeshSurface.h"

class FirstDemoScene : public CSE::Scene {
public:
    FirstDemoScene();
    ~FirstDemoScene() override;

    void Init() override;
    void Tick(float elapsedTime) override;
    void Destroy() override;

private:
};