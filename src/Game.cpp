#include "Game.h"
#include "Engine.h"
#include "Errors.h"
#include "ResourceManager.h"
#include "Level.h"
#include "Zombie.h"
#include "Human.h"

#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

Game::Game() {
}

Game::~Game() {
    for (auto level : levels) {
	delete level;
    }
}

void Game::Run() {
    InitSystems();
    InitLevel();

    auto music = audioManager.LoadMusic("data/sounds/No More Magic.mp3");
    music.Play(-1);

    RunGameLoop();
}

void Game::InitSystems() {
    Engine::Init();

    audioManager.Init();

    window.Create("Game Engine", screenWidth, screenHeight, 0);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    InitShaders();
    agentSpriteBatch.Init();

    camera.Init(screenWidth, screenHeight);
    camera.SetScale(0.5f);

    fpsLimiter.Init(maxFPS);

    Texture particleTexture = ResourceManager::GetTexture("data/textures/particle.png");
    bloodParticleBatch = new ParticleBatch(1'000, 0.05f, particleTexture, [](Particle& particle, float dt) {
	particle.position += particle.velocity * dt;
	particle.color.a = static_cast<GLubyte>(particle.life * 255.0f);
    });
    particleManager.AddParticleBatch(bloodParticleBatch);
}

void Game::InitLevel() {
    levels.push_back(new Level("data/level_1.txt"));
    currentLevel = 0;

    player = new Player();
    player->Init(5.0f, levels[currentLevel]->GetPlayerStartPosition(), &inputManager);
    humans.push_back(player);

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));
    std::uniform_int_distribution<int> randX(2, levels[currentLevel]->GetWidth() - 2);
    std::uniform_int_distribution<int> randY(2, levels[currentLevel]->GetHeight() - 2);

    for (int i = 0; i < levels[currentLevel]->GetNumHumans(); i++) {
	humans.push_back(new Human);
	glm::vec2 pos(randX(randomEngine) * TileSize, randY(randomEngine) * TileSize);
	humans.back()->Init(1.0f, pos);
    }

    const std::vector<glm::vec2>& zombiePositions = levels[currentLevel]->GetZombieStartPositions();
    for (auto& position : zombiePositions) {
	zombies.push_back(new Zombie);
	zombies.back()->Init(2.0f, position);
    }
}

void Game::InitShaders() {
    shader.Compile("data/shaders/TextureShading.vert", "data/shaders/TextureShading.frag");
    shader.AddAttribute("vertexPosition");
    shader.AddAttribute("vertexColor");
    shader.AddAttribute("vertexUV");
    shader.Link();
}

void Game::UpdateAgents(float deltaTime) {
    for (auto agent : humans) {
	agent->Update(levels[currentLevel]->GetLevelData(), humans, zombies, deltaTime);
    }

    for (auto agent : zombies) {
	agent->Update(levels[currentLevel]->GetLevelData(), humans, zombies, deltaTime);
    }

    for (int i = 0; i < zombies.size(); i++) {
	for (int j = i + 1; j < zombies.size(); j++) {
	    zombies[i]->CollideWithAgent(zombies[j]);
	}
	for (int j = 1; j < humans.size(); j++) {
	    if (zombies[i]->CollideWithAgent(humans[j])) {
		zombies.push_back(new Zombie);
		zombies.back()->Init(2.0f, humans[j]->GetPosition());
		delete humans[j];
		humans[j] = humans.back();
		humans.pop_back();
	    }
	}

	if (zombies[i]->CollideWithAgent(player)) {
	    AddBlood(player->GetPosition(), 1);
	}
    }

    for (int i = 0; i < humans.size(); i++) {
	for (int j = i + 1; j < humans.size(); j++) {
	    humans[i]->CollideWithAgent(humans[j]);
	}
    }
}

void Game::RunGameLoop() {
    const float millisPerSecond = 1000.0f;
    const float maxSteps = 6;
    const float targetFPS = 60.0f;
    const float targetFrameTime = millisPerSecond / targetFPS;
    const float maxDeltaTime = 1.0f;

    float previousTicks = SDL_GetTicks();

    int count = 0;
    while (gameState != GameState::Exit) {
	fpsLimiter.Begin();

	float newTicks = SDL_GetTicks();
	float frameTime = newTicks - previousTicks;
	previousTicks = newTicks;
	float totalDeltaTime = frameTime / targetFrameTime;

	ProcessInput();

	int i = 0;
	while (totalDeltaTime >= 0.0f && i < maxSteps) {
	    float deltaTime = std::min(totalDeltaTime, maxDeltaTime);
	    UpdateAgents(deltaTime);
	    totalDeltaTime -= deltaTime;
	    particleManager.Update(deltaTime);
	    i++;
	}

	camera.SetPosition(player->GetPosition());
	camera.Update();
	
	DrawGame();

	fps = fpsLimiter.End();
	count++;
	if (count == 30) {
	    std::cout << "fps: " << fps << std::endl;
	    count = 0;
	}
    }	
}

void Game::ProcessInput() {
    SDL_Event event;

    inputManager.Update();

    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	    gameState = GameState::Exit;
	    break;
	case SDL_KEYDOWN:
	    inputManager.PressKey(event.key.keysym.sym);
	    break;
	case SDL_KEYUP:
	    inputManager.ReleaseKey(event.key.keysym.sym);
	    break;
	case SDL_MOUSEBUTTONDOWN:
	    inputManager.PressKey(event.button.button);
	    break;
	case SDL_MOUSEBUTTONUP:
	    inputManager.ReleaseKey(event.button.button);
	    break;
	case SDL_MOUSEMOTION:
	    inputManager.SetMouseCoords(event.motion.x, event.motion.y);
	    break;
	}
    }
}

void Game::DrawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    // Looks like these 3 lines are optional.
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = shader.GetUniformLocation("spriteTexture");
    glUniform1i(textureUniform, 0);

    glm::mat4 projectionMatrix = camera.GetCameraMatrix();
    GLuint pUniform = shader.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    levels[currentLevel]->Draw();

    agentSpriteBatch.Begin();
    for (auto human : humans) {
	if (camera.IsBoxInView(human->GetPosition(), glm::vec2(AgentSize, AgentSize))) {
	    human->Draw(agentSpriteBatch);
	}
    }
    for (auto zombie : zombies) {
	if (camera.IsBoxInView(zombie->GetPosition(), glm::vec2(AgentSize, AgentSize))) {
	    zombie->Draw(agentSpriteBatch);
	}
    }    
    agentSpriteBatch.End();
    agentSpriteBatch.DrawBatch();

    particleManager.Draw(&agentSpriteBatch);

    shader.Unuse();

    window.SwapBuffer();
}

void Game::AddBlood(const glm::vec2& position, int numParticles) {
    static std::mt19937  randEngine(time(nullptr));
    static std::uniform_real_distribution<float> randomAngle(0.0f, 360.0f);

    glm::vec2 velocity(2.0f, 0.1f);
    glm::rotate(velocity, randomAngle(randEngine));
    ColorRGBA8 color(255.0f, 0.0f, 0.0f, 255.0f);

    for (int i = 0; i < numParticles; i++) {
	bloodParticleBatch->AddParticle(position, glm::rotate(velocity, randomAngle(randEngine)), color, 20);
    }
}
