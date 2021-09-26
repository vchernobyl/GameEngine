#include "ParticleManager.h"
#include "ParticleBatch.h"
#include "SpriteBatch.h"

ParticleManager::ParticleManager() { }

ParticleManager::~ParticleManager() {
    for (auto& batch : batches) {
	delete batch;
    }
}

void ParticleManager::AddParticleBatch(ParticleBatch* particleBatch) {
    batches.push_back(particleBatch);
}

void ParticleManager::Update(float deltaTime) {
    for (auto& batch : batches) {
	batch->Update(deltaTime);
    }
}

void ParticleManager::Draw(SpriteBatch* spriteBatch) {
    for (auto& batch : batches) {
	spriteBatch->Begin();
	batch->Draw(spriteBatch);
	spriteBatch->End();
	spriteBatch->DrawBatch();
    }
}
