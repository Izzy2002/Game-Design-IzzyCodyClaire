#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "ball.h"

int main(int argc, char** argv){
	int opt;
	while((opt = getopt(argc, argv, "h")) != -1){

	}
	Scene scene;
	Engine* engine = Engine::getInstance();
	PhysicsWorld physics(b2Vec2(0.0, -10.0f));

	b2BodyDef groundDef;
	groundDef.position.Set(0.0f, -7.6f);
	groundDef.type=b2_staticBody;
	b2Body* ground = physics.addBody(&groundDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 1.0f);
	ground->CreateFixture(&groundBox, 1.0f);


	b2BodyDef leftDef;
	leftDef.position.Set(0.0f, -7.6f);
	leftDef.type=b2_staticBody;
	b2Body* left = physics.addBody(&leftDef);
	b2PolygonShape leftBox;
	leftBox.SetAsBox(1.0f, 50.0f);
	left->CreateFixture(&leftBox, 1.0f);

	b2BodyDef rightDef;
	rightDef.position.Set(10.0f, -7.6f);
	rightDef.type=b2_staticBody;
	b2Body* right = physics.addBody(&rightDef);
	b2PolygonShape rightBox;
	rightBox.SetAsBox(1.0f, 50.0f);
	right->CreateFixture(&rightBox, 1.0f);

	for(int i=0; i<500; ++i){
		Ball* b = new Ball(&physics);
		scene.addUpdateable(*b);
		scene.addDrawable(*b);
	}

	scene.addUpdateable(physics);
	engine->core_loop(scene);
	engine->shutdown();
}
