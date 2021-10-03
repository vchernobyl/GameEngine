#include "BoxCollider.h"
#include "SpriteBatch.h"

#include <box2d/box2d.h>

BoxCollider::BoxCollider() {
}

BoxCollider::BoxCollider(class b2World* world, const glm::vec2& position, const glm::vec2& size,
			 Texture texture, const ColorRGBA8& color, bool fixedRotation,
			 glm::vec4 uv)
    : size(size), color(color), texture(texture), uv(uv) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = fixedRotation;

    body = world->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    fixture = body->CreateFixture(&fixtureDef);
}

void BoxCollider::Draw(SpriteBatch& spriteBatch) {
    glm::vec2 size = GetSize();
    glm::vec2 position = GetPosition();
    glm::vec4 destRect(position.x - size.x / 2.0f, position.y - size.y / 2.0f, size.x, size.y);

    spriteBatch.Draw(destRect,
		     uv,
		     texture.id,
		     0.0f,
		     GetColor(),
		     GetAngle());
}

glm::vec2 BoxCollider::GetPosition() const {
    b2Vec2 position = body->GetPosition();
    return glm::vec2(position.x, position.y);
}

float BoxCollider::GetAngle() const {
    return body->GetAngle();
}

void BoxCollider::ApplyForceToCenter(const glm::vec2& force) {
    body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void BoxCollider::ApplyImpulse(const glm::vec2& impulse) {
    body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(0.0f, 0.0f), true);
}
