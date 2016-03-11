#ifndef PHYSICS_WORLD
#define PHYSICS_WORLD

#include "Box2D\Box2D.h"
#include "TinyXML\tinyxml.h"
#include "AngryBirdsContactListener.h"

// user data to be stored in the Box2D physics body
struct _BodyUserData
{
	int bodyId;
	int bodyHealth;
	bool collided;
	bool specialPowerUsed;
};

typedef struct _BodyUserData BodyUserData;

/**
 *	@class PhysicsWorld
 *	@desc  Singleton storing the box2d world and physics properties
 */
class PhysicsWorld
{
	public:
		~PhysicsWorld();
		static PhysicsWorld* getInstance();
		static b2World* world;
		void init(TiXmlElement* physicsElement);
		void update(unsigned long miliseconds);
		void explode(b2Vec2 center, float blastRadius);
		bool isWorldSleeping();
		void setSleeping(bool value);
		static const int RATIO = 40;

	private:
		PhysicsWorld(){};
		static PhysicsWorld *sInstance;

		void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

		float32 timeStep;
		int32 velocityIterations;
		int32 positionIterations;
		
		AngryBirdsContactListener* contactListener;
		
		unsigned long deltaTime;
		unsigned long updateDeltaTime;
		unsigned int startTime;
		bool sleeping;

		const double numRadialRayCasts = 30;
		const double blastPower = 10000;
		const unsigned long collisionStartTime = 2000;
};

#endif