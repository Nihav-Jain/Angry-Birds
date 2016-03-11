#include "stdafx.h"
#include "WoodBlock.h"

WoodBlock::WoodBlock()
{

}


void WoodBlock::init(int x, int y, float wid, float ht, float angDeg, ObjectData blockObjectData)
{
	blockUserData.bodyHealth = blockObjectData.health;
	blockUserData.bodyId = 102;
	blockUserData.collided = false;

	maxHealth = blockObjectData.health;

	renderIndex = blockObjectData.spriteData[0].rendererIndex;

	width = wid;
	height = ht;

	isAlive = true;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x / PhysicsWorld::RATIO, y / PhysicsWorld::RATIO);
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = &blockUserData;
	//bodyDef.fixedRotation = false;

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox((width / 2) / PhysicsWorld::RATIO, (height / 2) / PhysicsWorld::RATIO); //a 4x2 rectangle

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &polygonShape;
	bodyFixture.friction = blockObjectData.friction;
	bodyFixture.restitution = blockObjectData.restitution;
	bodyFixture.density = blockObjectData.density;

	body = PhysicsWorld::world->CreateBody(&bodyDef);
	body->CreateFixture(&bodyFixture);
	body->SetTransform(body->GetPosition(), angDeg * 3.14159 / 180);
	
}

void WoodBlock::update()
{
	if (blockUserData.bodyHealth <= 0)
	{
		if (isAlive)
		{
			isAlive = false;
			PhysicsWorld::world->DestroyBody(body);
			body = NULL;
		}
	}

}

void WoodBlock::render()
{
	
}

b2Vec2 WoodBlock::getBodyPosition()
{
	return body->GetPosition();
}

float WoodBlock::getBodyAngleInRads()
{
	return body->GetAngle();
}

float WoodBlock::getBodyWidth()
{
	return width;
}

float WoodBlock::getBodyHeight()
{
	return height;
}


void WoodBlock::shutdown()
{
	if (isAlive)
	{
		PhysicsWorld::world->DestroyBody(body);
	}
}

int WoodBlock::getBlockRenderIndex()
{
	return renderIndex;
}

bool WoodBlock::isBlockAlive()
{
	return isAlive;
}

int WoodBlock::getBlockMaxHealth()
{
	return maxHealth;
}

int WoodBlock::getBlockHealth()
{
	return blockUserData.bodyHealth;
}