#include "mathfu/vector.h"
#include "ball.h"
#include "LEAGUE/physics.h"
#include "LEAGUE/engine.h"
#include <iostream>
#include <string>
#include <box2d/box2d.h>
#include <random>

Ball::Ball(PhysicsWorld* physics, float x, float y){

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, 10);

	// Generate a random number between 0 and RAND_MAX
	loadImage("./assets/randombox.png");
	// Need a body definition before we can make a body
	bodyDef = new b2BodyDef();
	bodyDef->type = b2_dynamicBody;
	bodyDef->position.Set(x, y);
	// Physics engine makes the body for us and returns a pointer to it
	body = physics->addBody(bodyDef);
	// Need a shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(.2f,.18f);
	// b2CircleShape ballShape;
	// ballShape.m_radius=0.16f;
	// Must apply a fixture.  Fixes shape and other properties to it.
	b2FixtureDef boxFixture;
	boxFixture.shape = &boxShape;
	boxFixture.density = 1.0f;
	boxFixture.friction = 0.3f;
	boxFixture.restitution = 0.3f;
	// b2FixtureDef ballFixture;
	// ballFixture.shape = &ballShape;
	// ballFixture.density = 1.0f;
	// ballFixture.friction = 0.3f;
	// ballFixture.restitution = 0.3f;
	// Make the fixture.
	// body->CreateFixture(&ballFixture);
	body->CreateFixture(&boxFixture);
}

Ball::~Ball(){
	delete bodyDef;
}

void Ball::setBody(b2Body* body){
	this->body = body;
}

b2BodyDef* Ball::getBodyDef(){
	return bodyDef;
}

void Ball::update(double delta){
	//std::cout << body->GetPosition().x << ", " << body->GetPosition().y << std::endl;
	auto events = Engine::getEvents();
	for(auto event=events.begin(); event!=events.end(); ++event){
		if(event->type == SDL_KEYDOWN){
			if(event->key.keysym.sym == SDLK_SPACE){
				b2Vec2 up(0.0f, 1.0f);
				b2Vec2 pos = body->GetPosition();
				body->ApplyLinearImpulse(up, pos, true);
				// body->ApplyTorque(5.0f, true);
			}
			if(event->key.keysym.sym == SDLK_LEFT){
				b2Vec2 left(-0.5f, 0.0f);
				b2Vec2 pos = body->GetPosition();
				body->ApplyLinearImpulse(left, pos, true);
			}
			if(event->key.keysym.sym == SDLK_RIGHT){
				b2Vec2 right(0.5f, 0.0f);
				b2Vec2 pos = body->GetPosition();
				body->ApplyLinearImpulse(right, pos, true);
			}
		}
		//old deletion code

		// if(event->type == SDL_MOUSEBUTTONUP){
		// 	b2Vec2 pos = body->GetPosition();
		// 	int temp, tempY;
		// 	SDL_GetMouseState(&temp, &tempY);
		// 	float x = (float)temp * .01;
		// 	float y = - (float)tempY * .01;
		// 	std::cout << x << ", " << y << std::endl;
		// 	std::cout << "Left side: " << pos.x - .2 << std::endl;
		// 	std::cout << "Right side: " << pos.x + .2 << std::endl;
		// 	std::cout << "Top side: " << pos.y +.18 << std::endl;
		// 	std::cout << "Bottom side: " << pos.y -.18 << std::endl;
		// 	if(x > pos.x - .2 && x < pos.x + .2 && y > pos.y -.18 && y < pos.y + .18){
		// 		std::cout << x << ", " << y << std::endl;
		// 		delete bodyDef;
		// 		SDL_DestroyTexture(texture);
		// 	}


			// b2Transform t;
			// t.SetIdentity();
			// b2Fixture* f = body->GetFixtureList(); 
			// b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
			// const int childCount = polygonShape->GetChildCount();
			// for (int child = 0; child < childCount; child++){
				
				
			// }
			
			
			
		}
	}	

void Ball::draw(SDL_Renderer* renderer){
	SDL_Rect dest;
	b2Vec2 pos = body->GetPosition();
	dest.x = pos.x * 100;
	dest.y = pos.y * -1 * 100;
	dest.w = rect.w;
	dest.h = rect.h;
	float angle = body->GetAngle();
	//std::cout << dest.x << ", " << dest.y << std::endl;
	int result = SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	if(result != 0){
		std::cout << SDL_GetError() << std::endl;
	}
	
}

b2Body* Ball::getBody(){
	return body;
}
