#ifndef RAYCAST_H
#define RAYCAST_H

#include "Box2D\Box2D.h"
using namespace std;

class RayCastClosestCallback : public b2RayCastCallback
{
public:
	b2Body* m_body;
	b2Vec2 m_point;

	RayCastClosestCallback() { m_body = NULL; }

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_body = fixture->GetBody();
		m_point = point;
		return fraction;
	}
};

class MyQueryCallback : public b2QueryCallback {
public:
	
	b2Body* foundBodies[50];
	int bodyCount;

	bool ReportFixture(b2Fixture* fixture) {
		//foundBodies.push_back(fixture->GetBody());
		foundBodies[bodyCount] = fixture->GetBody();
		bodyCount++;
		return true;//keep going to find all fixtures in the query area
	}
};

#endif