#include "ParticleBatch.h"
#include "SpriteBatch.h"

void Particle::Update(float deltaTime) {
    position += velocity * deltaTime;
}

ParticleBatch::ParticleBatch(int maxParticles, float decayRate, Texture texture) :
    maxParticles(maxParticles),
    decayRate(decayRate),
    particles(new Particle[maxParticles]),
    texture(texture) {
}

ParticleBatch::~ParticleBatch() {
    delete[] particles;
}

void ParticleBatch::Update(float deltaTime) {
    for (int i = 0; i < maxParticles; i++) {
	if (auto& particle = particles[i]; particle.life > 0.0f) {
	    particle.Update(deltaTime);
	    particle.life -= decayRate * deltaTime;
	}
    }
}

void ParticleBatch::Draw(SpriteBatch* spriteBatch) {
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    for (int i = 0; i < maxParticles; i++) {
	if (auto& particle = particles[i]; particle.life > 0.0f) {
	    glm::vec4 destRect(particle.position.x, particle.position.y,
			       particle.width, particle.width);
	    spriteBatch->Draw(destRect, uvRect, texture.id, 0.0f, particle.color);
	}
    }
}

void ParticleBatch::AddParticle(const glm::vec2& position,
				const glm::vec2& velocity,
				const ColorRGBA8& color,
				float width) {
    const int particleIndex = FindFreeParticle();
    auto& particle = particles[particleIndex];
    particle.life = 1.0f;
    particle.position = position;
    particle.velocity = velocity;
    particle.color = color;
    particle.width = width;
}

int ParticleBatch::FindFreeParticle() {
    for (int i = lastFreeParticle; i < maxParticles; i++) {
	if (particles[i].life <= 0.0f) {
	    lastFreeParticle = i;
	    return i;
	}
    }

    for (int i = 0; i < lastFreeParticle; i++) {
	if (particles[i].life <= 0.0f) {
	    lastFreeParticle = i;
	    return i;
	}
    }

    return 0;
}
