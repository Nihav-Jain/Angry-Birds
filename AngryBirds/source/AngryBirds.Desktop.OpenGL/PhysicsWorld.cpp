#include "stdafx.h"
#include "PhysicsWorld.h"
#include "RayCastClosestCallback.h"

PhysicsWorld* PhysicsWorld::sInstance = NULL;
b2World* PhysicsWorld::world = NULL;

/**
 *	@method PhysicsWorld::~PhysicsWorld()
 *	@desc   destructor to free up heap allocations
 */
PhysicsWorld::~PhysicsWorld()
{
	delete world;
}

/**
 *	@method PhysicsWorld::getInstance()
 *	@desc   returns the singleton instance
 */
PhysicsWorld* PhysicsWorld::getInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new PhysicsWorld();;
	}
	return sInstance;
}

/**
 *	@method PhysicsWorld::init()
 *	@desc   instantiates the box2d physics world and world update variables
 */
void PhysicsWorld::init(TiXmlElement* physicsElement)
{
	timeStep = 1.0f / 60.0f;
	velocityIterations = 6;
	positionIterations = 2;
	deltaTime = 0;
	startTime = collisionStartTime;
	sleeping = false;

	float gravityX, gravityY;
	physicsElement->QueryFloatAttribute("gravityX", &gravityX);
	physicsElement->QueryFloatAttribute("gravityY", &gravityY);

	int updateDeltaTime32;
	physicsElement->QueryIntAttribute("updateDeltaTime", &updateDeltaTime32);
	updateDeltaTime = updateDeltaTime32;

	b2Vec2 gravity;
	gravity.Set(gravityX, gravityY);
	PhysicsWorld::world = new b2World(gravity);

	contactListener = new AngryBirdsContactListener();
	PhysicsWorld::world->SetContactListener(contactListener);
}

/**
 *	@method PhysicsWorld::update()
 *	@desc   called every frame, calls the Box2D world update on a fixed time basis
 */
void PhysicsWorld::update(unsigned long miliseconds)
{
	deltaTime += miliseconds;

	if (startTime < collisionStartTime)
	{
		startTime += miliseconds;
		if (startTime >= collisionStartTime)
		{
			sleeping = false;
		}
	}

	if (deltaTime > updateDeltaTime)
	{
		PhysicsWorld::world->Step(timeStep, velocityIterations, positionIterations);
		deltaTime = deltaTime % updateDeltaTime;
	}
}

void PhysicsWorld::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower)
{
	BodyUserData* userData = static_cast<BodyUserData *>(body->GetUserData());

	b2Vec2 blastDir = applyPoint - blastCenter;
	float distance = blastDir.Normalize();

	//ignore bodies exactly at the blast point - blast direction is undefined
	if (distance == 0 || userData->bodyId == 100)
		return;

	float invDistance = 1 / distance;
	float impulseMag = blastPower * invDistance * invDistance;

	userData->bodyHealth -= impulseMag;

	body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
}

void PhysicsWorld::explode(b2Vec2 center, float blastRadius)
{

	MyQueryCallback queryCallback;
	queryCallback.bodyCount = 0;

	b2AABB aabb;
	aabb.lowerBound = center - b2Vec2(blastRadius, blastRadius);
	aabb.upperBound = center + b2Vec2(blastRadius, blastRadius);
	world->QueryAABB(&queryCallback, aabb);

	for (int i = 0; i < queryCallback.bodyCount; i++) {
		b2Body* body = queryCallback.foundBodies[i];
		b2Vec2 bodyCom = body->GetWorldCenter();

		//ignore bodies outside the blast range
		if ((bodyCom - center).Length() >= 100)
			continue;

		applyBlastImpulse(body, center, bodyCom, blastPower);
	}

	//for (int i = 0; i < numRadialRayCasts; i++)
	//{
	//	float angle = (i / (float)numRadialRayCasts) * 360 * 3.14159265 / 180;
	//	b2Vec2 rayDir(sinf(angle), cosf(angle));
	//	b2Vec2 rayEnd = center + blastRadius * rayDir;

	//	//check what this ray hits
	//	RayCastClosestCallback callback;//basic callback to record body and hit point
	//	world->RayCast(&callback, center, rayEnd);
	//	if (callback.m_body)
	//		applyBlastImpulse(callback.m_body, center, callback.m_point, (blastPower / (float)numRadialRayCasts));
	//}
}

bool PhysicsWorld::isWorldSleeping()
{
	return sleeping;
}

void PhysicsWorld::setSleeping(bool value)
{
	sleeping = value;
	if (sleeping)
	{
		startTime = 0;
	}
}

