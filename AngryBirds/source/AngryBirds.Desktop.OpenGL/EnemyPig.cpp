#include "stdafx.h"
#include "EnemyPig.h"

void EnemyPig::init(int x, int y, float wid, ObjectData pigObjectData)
{
	pigUserData.bodyHealth = pigObjectData.health;
	pigUserData.bodyId = 101;
	pigUserData.collided = false;

	maxHealth = pigObjectData.health;

	renderIndex = pigObjectData.spriteData[0].rendererIndex;

	width = wid;
	height = width;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x / PhysicsWorld::RATIO, y / PhysicsWorld::RATIO);
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = &pigUserData;

	b2CircleShape bodyShape;
	bodyShape.m_radius = (width / 2.0) / PhysicsWorld::RATIO;

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = pigObjectData.friction;
	bodyFixture.restitution = pigObjectData.restitution;
	bodyFixture.density = pigObjectData.density;

	body = PhysicsWorld::world->CreateBody(&bodyDef);
	body->CreateFixture(&bodyFixture);

	poofing = false;
	poofX = 0;
	poofY = 0;
	poofAnimationFrame = 0;
	poofAnimationDeltaTime = 0;

	pigAlive = true;
}

void EnemyPig::update()
{
	if (pigAlive)
	{
		float posX = body->GetPosition().x * PhysicsWorld::RATIO;
		if (posX < 0 || posX > 1980)
		{
			pigUserData.bodyHealth = 0;
		}
	}

	if (pigUserData.bodyHealth <= 0)
	{
		// kill pig
		if (pigAlive)
		{
			poofing = true;
			poofAnimationFrame = 0;
			poofAnimationDeltaTime = 0;
			poofX = body->GetPosition().x * PhysicsWorld::RATIO;
			poofY = body->GetPosition().y * PhysicsWorld::RATIO;

			SoundManagerC::GetInstance()->PlayDeadPig();
			PhysicsWorld::world->DestroyBody(body);
			body = NULL;
			pigAlive = false;
			CGame::GetInstance()->decrementPigCount();

		}
	}
}

void EnemyPig::render()
{


}

bool EnemyPig::isPigAlive()
{
	return pigAlive;
}

b2Vec2 EnemyPig::getPigBodyPosition()
{
	return body->GetPosition();
}

float EnemyPig::getPigBodyAngleInRads()
{
	return body->GetAngle();
}

float EnemyPig::getPigBodyWidth()
{
	return width;
}

int EnemyPig::getPigHealth()
{
	return pigUserData.bodyHealth;
}


void EnemyPig::shutdown()
{
	if (pigAlive)
	{
		PhysicsWorld::world->DestroyBody(body);
		body = NULL;
		pigAlive = false;
	}

}

int EnemyPig::getPigRenderIndex()
{
	return renderIndex;
}

int EnemyPig::getPigMaxHealth()
{
	return maxHealth;
}

bool EnemyPig::isPoofing()
{
	return poofing;
}

int EnemyPig::getPoofAnimationFrame()
{
	return poofAnimationFrame;
}

void EnemyPig::setPoofAnimationFrame(int value)
{
	poofAnimationFrame = value;
}

float EnemyPig::getPoofX()
{
	return poofX;
}

float EnemyPig::getPoofY()
{
	return poofY;
}

void EnemyPig::setPoofAnimationDeltaTime(DWORD value)
{
	poofAnimationDeltaTime = value;
}

DWORD EnemyPig::getPoofAnimationDeltaTime()
{
	return poofAnimationDeltaTime;
}