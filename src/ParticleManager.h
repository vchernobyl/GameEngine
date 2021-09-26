#pragma once

#include "SpriteBatch.h"
#include <vector>

class ParticleBatch;
class SpriteBatch;
class ParticleManager {
public:
    ParticleManager();
    ~ParticleManager();
    void AddParticleBatch(ParticleBatch* particleBatch);
    void Update(float deltaTime);
    void Draw(SpriteBatch* spriteBatch);

private:
    std::vector<ParticleBatch*> batches;
};
