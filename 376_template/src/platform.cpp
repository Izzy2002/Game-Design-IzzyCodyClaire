#include "mathfu/vector.h"
#include "platform.h"
#include "LEAGUE/physics.h"
#include "LEAGUE/engine.h"
#include <iostream>
#include <string>
#include <box2d/box2d.h>

Platform::Platform(PhysicsWorld* physics, float x, float y){

	loadImage("./assets/tile_0002.png");
	// Need a body definition before we can make a body
	bodyDef = new b2BodyDef();
	bodyDef->type = b2_staticBody;
	bodyDef->position.Set(x, y);
	// Physics engine makes the body for us and returns a pointer to it
	body = physics->addBody(bodyDef);
	// Need a shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(.1f,.1f);
	// Must apply a fixture.  Fixes shape and other properties to it.
	b2FixtureDef boxFixture;
	boxFixture.shape = &boxShape;
	boxFixture.density = 1.0f;
	boxFixture.friction = 0.9f;

	body->CreateFixture(&boxFixture);
}

Platform::~Platform(){
	delete bodyDef;
}

void Platform::setBody(b2Body* body){
	this->body = body;
}

b2BodyDef* Platform::getBodyDef(){
	return bodyDef;
}

void Platform::draw(SDL_Renderer* renderer){
	SDL_Rect dest;
	b2Vec2 pos = body->GetPosition();
	dest.x = pos.x * 100;
	dest.y = pos.y * -1 * 100;
	dest.w = rect.w;
	dest.h = rect.h;
	float angle = body->GetAngle();
	int result = SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	if(result != 0){
		std::cout << SDL_GetError() << std::endl;
	}
}

b2Body* Platform::getBody(){
	return body;
}
