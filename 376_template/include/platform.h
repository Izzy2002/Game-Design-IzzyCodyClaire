#ifndef		__H__PLATFORM__
#define		__H__PLATFORM__

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/physics.h"
#include <box2d/box2d.h>

class Platform : public Drawable{
	public:
		Platform(PhysicsWorld*, float x, float y);
		~Platform();
		void draw(SDL_Renderer* renderer) override;
		float flip(float);
		void setBody(b2Body*);
		b2BodyDef* getBodyDef();
		b2Body* getBody();
	private:
		int x_vel;
		int y_vel;
		b2Body* body;
		b2BodyDef* bodyDef;
};

#endif
