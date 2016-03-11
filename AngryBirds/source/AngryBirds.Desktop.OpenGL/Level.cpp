#include "stdafx.h"
#include "Level.h"

/**
 *	@method Level::init()
 *	@desc   initializes the Box2D terrain for the level
 */
void Level::init()
{
	groundUserData.bodyHealth = 100;
	groundUserData.bodyId = 103;

	width = 1920;
	height = 155*2;

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set((1920 / 2) / PhysicsWorld::RATIO, 0);
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.userData = &groundUserData;

	b2PolygonShape bodyShape;
	bodyShape.SetAsBox((1920 / 2) / PhysicsWorld::RATIO, 155 / PhysicsWorld::RATIO);

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = 100;
	bodyFixture.restitution = 0.3;
	bodyFixture.density = 10;

	groundBody = PhysicsWorld::world->CreateBody(&groundBodyDef);
	groundBody->CreateFixture(&bodyFixture);
}

/**
 *	@method Level::update()
 *	@desc   called every frame (currently unused)
 */
void Level::update(unsigned long millis)
{

}

/**
 *	@method Level::render()
 *	@desc   calls the renderer for the terrain (currently unused)
 */
void Level::render()
{
	
}

/**
 *	@method Level::~Level()
 *	@desc   destuctor to destory box2D bodies and free up heap allocations
 */
Level::~Level()
{
	PhysicsWorld::world->DestroyBody(groundBody);
}
