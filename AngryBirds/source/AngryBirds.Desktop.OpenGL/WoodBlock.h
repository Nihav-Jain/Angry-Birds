#ifndef WOOD_BLOCK_H
#define WOOD_BLOCK_H

#include "HashMap.h"
#include "PhysicsWorld.h"

class WoodBlock
{
public:
	WoodBlock();
	void init(int x, int y, float wid, float ht, float angDeg, ObjectData blockObjectData);
	void update();
	void render();
	void shutdown();
	bool isBlockAlive();

	b2Vec2 getBodyPosition();
	float getBodyAngleInRads();
	float getBodyWidth();
	float getBodyHeight();
	int getBlockRenderIndex();
	int getBlockMaxHealth();
	int getBlockHealth();
	//~WoodBlock();

private:
	b2Body *body;
	float width;
	float height;
	BodyUserData blockUserData;

	int renderIndex;
	bool isAlive;
	int maxHealth;
	
};

#endif