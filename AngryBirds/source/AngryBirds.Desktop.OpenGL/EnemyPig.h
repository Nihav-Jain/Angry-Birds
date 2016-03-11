#ifndef ENEMY_PIG_MANAGER
#define ENEMY_PIG_MANAGER

#include "Box2D\Box2D.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "HashMap.h"

/**
 *	@class EnemyPig
 *	@desc Handles the physics body and health status of a pig
 */
class EnemyPig
{
	public:
		void init(int x, int y, float wid, ObjectData pigObjectData);
		void update();
		void render();
		bool isPigAlive();
		void shutdown();

		b2Vec2 getPigBodyPosition();
		float getPigBodyAngleInRads();
		float getPigBodyWidth();
		int getPigHealth();
		int getPigRenderIndex();
		int getPigMaxHealth();

		bool isPoofing();
		int getPoofAnimationFrame();
		void setPoofAnimationFrame(int value);
		float getPoofX();
		float getPoofY();
		DWORD getPoofAnimationDeltaTime();
		void setPoofAnimationDeltaTime(DWORD value);

		//~EnemyPig();
	private:

		b2Body* body;

		int width;
		int height;
		BodyUserData pigUserData;
		bool pigAlive;
		int renderIndex;
		Renderer* PigRenderer;
		int maxHealth;

		int poofAnimationFrame;
		bool poofing;
		float poofX;
		float poofY;
		DWORD poofAnimationDeltaTime;

};



#endif