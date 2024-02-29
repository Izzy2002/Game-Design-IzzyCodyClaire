#ifndef		__H__PLAYER__
#define		__H__PLAYER__

#include "LEAGUE/common_data_structures.h"
#include <box2d/box2d.h>

class Player : public Drawable, public Updateable {
	public:
		Player();
		~Player();
		void draw(SDL_Renderer* renderer) override;
		void update(double delta) override;
		float flip(float);
		void setBody(b2Body*);
		b2BodyDef* getBodyDef();
	private:
		int x_vel;
		int y_vel;
		b2Body* body;
		b2BodyDef* bodyDef;
};

class Background : public Drawable, public Updateable {
	public:
		Background(const char* path);
		void draw(SDL_Renderer* renderer) override;
		void update(double delta) override;	
};

#endif
