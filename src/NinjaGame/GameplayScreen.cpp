#include "GameplayScreen.h"

#include "../IGame.h"
#include "../ResourceManager.h"
#include "../Window.h"
#include "../Vertex.h"

#include <iostream>
#include <random>
#include <GL/glew.h>

GameplayScreen::GameplayScreen(Window* window) : window(window) {
}

void GameplayScreen::Build() { }

void GameplayScreen::Destroy() { }

void GameplayScreen::OnEntry() {
    b2Vec2 gravity(0.0f, -9.8f);
    world = std::make_unique<b2World>(gravity);

    // Define the ground.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -25.0f);

    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Make random boxes.
    std::mt19937 randomGenerator;
    std::uniform_real_distribution<float> xDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> yDist(-10.0f, 15.0f);
    std::uniform_real_distribution<float> sizeDist(0.5f, 2.5f);
    std::uniform_real_distribution<float> colorDist(0.0f, 255.0f);
    
    const int numBoxes = 50;

    for (int i = 0; i < numBoxes; i++) {
	ColorRGBA8 color;
	color.r = colorDist(randomGenerator);
	color.g = colorDist(randomGenerator);
	color.b = colorDist(randomGenerator);
	color.a = 255.0f;
	BoxCollider box(world.get(),
			glm::vec2(xDist(randomGenerator), yDist(randomGenerator)),
			glm::vec2(sizeDist(randomGenerator), sizeDist(randomGenerator)),
			color);
	boxes.push_back(box);
    }	


    // Sprite batch init.
    spriteBatch.Init();

    // Shader init.
    textureShader.Compile("data/shaders/TextureShading.vert", "data/shaders/TextureShading.frag");
    textureShader.AddAttribute("vertexPosition");
    textureShader.AddAttribute("vertexColor");
    textureShader.AddAttribute("vertexUV");
    textureShader.Link();

    texture = ResourceManager::GetTexture("data/textures/red_bricks.png");

    // Camera init.
    camera.Init(window->GetScreenWidth(), window->GetScreenHeight());
    camera.SetScale(16.0f);
}

void GameplayScreen::OnExit() { }

void GameplayScreen::Update() {
    ProcessInput();
    world->Step(1.0f / 60.0f, 6, 2);
    camera.Update();
}

void GameplayScreen::Draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    textureShader.Use();

    // Upload texture uniform.
    GLint textureUniform = textureShader.GetUniformLocation("spriteTexture");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    // Set camera projection.
    glm::mat4 projectionMatrix = camera.GetCameraMatrix();
    GLint projectionUniform = textureShader.GetUniformLocation("P");
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    spriteBatch.Begin();

    // Draw all the boxes.
    for (auto& b : boxes) {
	glm::vec2 size = b.GetSize();
	glm::vec2 position = b.GetPosition();
	glm::vec4 destRect(position.x - size.x / 2.0f, position.y - size.y / 2.0f, size.x, size.y);

	spriteBatch.Draw(destRect,
			 glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			 texture.id,
			 0.0f,
			 b.GetColor(),
			 b.GetAngle());
    }

    spriteBatch.End();
    spriteBatch.DrawBatch();
    textureShader.Unuse();
}

int GameplayScreen::GetNextScreenIndex() const {
    return -1;
}

int GameplayScreen::GetPreviousScreenIndex() const {
    return -1;
}

void GameplayScreen::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	game->OnSdlEvent(event);
    }
}
