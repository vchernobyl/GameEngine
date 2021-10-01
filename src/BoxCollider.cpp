#include "BoxCollider.h"
#include <box2d/box2d.h>

BoxCollider::BoxCollider(class b2World* world, const glm::vec2& position, const glm::vec2& size,
			 const ColorRGBA8& color)
    : size(size), color(color) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);

    body = world->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    fixture = body->CreateFixture(&fixtureDef);
}

glm::vec2 BoxCollider::GetPosition() const {
    b2Vec2 position = body->GetPosition();
    return glm::vec2(position.x, position.y);
}

float BoxCollider::GetAngle() const {
    return body->GetAngle();
}
