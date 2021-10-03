#include "Player.h"
#include "../ResourceManager.h"

#include <SDL/SDL.h>
#include <box2d/box2d.h>
#include <iostream>

Player::Player() {
}

Player::Player(b2World* world, const glm::vec2& position, const glm::vec2& size,
	       const ColorRGBA8& color)
    : boxCollider(world,
		  position,
		  size,
		  ResourceManager::GetTexture("data/textures/Ninja/ninja_full.png"),
		  color, true, glm::vec4(0.0f, 0.9f, 0.1f, 0.1f)) {
}

void Player::Update(const InputManager& inputManager) {
    if (inputManager.IsKeyDown(SDLK_a)) {
	boxCollider.ApplyForceToCenter(glm::vec2(-100.0f, 0.0f));
    } else if (inputManager.IsKeyDown(SDLK_d)) {
	boxCollider.ApplyForceToCenter(glm::vec2(100.0f, 0.0f));
    } else {
	b2Body* body = boxCollider.GetBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	body->SetLinearVelocity(b2Vec2(velocity.x * 0.95f, velocity.y));
    }

    float maxSpeed = 10.0f;
    b2Body* body = boxCollider.GetBody();
    b2Vec2 velocity = body->GetLinearVelocity();
    if (velocity.x < -maxSpeed) {
	body->SetLinearVelocity(b2Vec2(-maxSpeed, velocity.y));
    } else if (velocity.x > maxSpeed) {
	body->SetLinearVelocity(b2Vec2(maxSpeed, velocity.y));
    }

    for (b2ContactEdge* edge = body->GetContactList(); edge != nullptr; edge = edge->next) {
	b2Contact* contact = edge->contact;
	if (contact->IsTouching()) {
	    b2WorldManifold manifold;
	    contact->GetWorldManifold(&manifold);

	    // Check if points are below.
	    bool below = false;
	    for (int i = 0; i < b2_maxManifoldPoints; i++) {
		if (manifold.points[i].y < body->GetPosition().y - boxCollider.GetSize().y / 2.0f + 0.01f) {
		    below = true;
		    break;
		}
	    }

	    if (below) {
		if (inputManager.IsKeyPressed(SDLK_w)) {
		    boxCollider.ApplyImpulse(glm::vec2(0.0f, 35.0f));
		    break;
		}
	    }
	}
    }
}

void Player::Draw(SpriteBatch& spriteBatch) {
    boxCollider.Draw(spriteBatch);
}
