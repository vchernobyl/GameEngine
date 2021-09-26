#pragma once

#include "Vertex.h"
#include "Texture.h"
#include <glm/glm.hpp>

class Particle {
public:
    void Update(float deltaTime);
private:
    friend class ParticleBatch;
    
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 velocity = glm::vec2(0.0f);
    ColorRGBA8 color;
    float life = 0.0f;
    float width = 0.0f;
};

class ParticleBatch {
public:
    ParticleBatch(int maxParticles, float decayRate, Texture texture);
    ~ParticleBatch();

    void Update(float deltaTime);
    void Draw(class SpriteBatch* spriteBatch);
    
    void AddParticle(const glm::vec2& position,
		     const glm::vec2& velocity,
		     const ColorRGBA8& color,
		     float width);
    
private:
    int FindFreeParticle();

    int maxParticles = 0;
    int lastFreeParticle = 0;
    float decayRate = 0.1f;
    Particle* particles = nullptr;
    Texture texture;
};
