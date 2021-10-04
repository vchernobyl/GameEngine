#pragma once

#include "../IGameScreen.h"
#include "../BoxCollider.h"
#include "../SpriteBatch.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Texture.h"
#include "../DebugRenderer.h"

#include "Player.h"

#include <box2d/box2d.h>
#include <memory>
#include <vector>

class GameplayScreen : public IGameScreen {
public:
    GameplayScreen(class Window* window);

    void Build() override;
    void Destroy() override;

    void OnEntry() override;
    void OnExit() override;

    void Update() override;
    void Draw() override;

    int GetNextScreenIndex() const override;
    int GetPreviousScreenIndex() const override;

private:
    void ProcessInput();

    SpriteBatch spriteBatch;
    Shader textureShader;
    Camera camera;
    Texture texture;
    class Window* window;
    DebugRenderer debugRenderer;

    std::unique_ptr<b2World> world;
    std::vector<BoxCollider> boxes;

    Player player;
};
