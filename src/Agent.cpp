#include "Agent.h"
#include "ResourceManager.h"

Agent::Agent() {
}

Agent::~Agent() {
}

void Agent::Draw(SpriteBatch& spriteBatch) {
    static int textureID = ResourceManager::GetTexture("data/textures/circle.png").id;
    spriteBatch.Draw(glm::vec4(position.x, position.y, AgentSize, AgentSize),
		     glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		     textureID,
		     0.0f,
		     color);
}
