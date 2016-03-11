#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include "Box2D\Box2D.h"

/**
 *	@class AngryBirdsContactListener
 *	@desc  custom Box2D contact listener for calculating damage t pigs, blocks and bird
 */
class AngryBirdsContactListener : public b2ContactListener
{
	public:
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif