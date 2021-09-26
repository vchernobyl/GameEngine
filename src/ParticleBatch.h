#pragma once

#include "Vertex.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <functional>

struct Particle {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 velocity = glm::vec2(0.0f);
    ColorRGBA8 color;
    float life = 0.0f;
    float width = 0.0f;
};

inline void DefaultParticleUpdate(Particle& particle, float deltaTime) {
    particle.position += particle.velocity * deltaTime;
}

class ParticleBatch {
public:
    ParticleBatch(int maxParticles, float decayRate, Texture texture,
		  std::function<void(Particle&, float)> updateFunc = DefaultParticleUpdate);
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
    std::function<void(Particle&, float)> updateFunc;
};
