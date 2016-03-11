#include "stdafx.h"
#include "AngryBirdsContactListener.h"

/**
 *	@method AngryBirdsContactListener::PostSolve
 *	@desc called after the contact data has been solved, provides data like the impulse betwen the colliding bodies
 */
void AngryBirdsContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (PhysicsWorld::getInstance()->isWorldSleeping())
		return;

	if (impulse->normalImpulses[0] > 10)
	{
		BodyUserData* userDataA = static_cast<BodyUserData*>(contact->GetFixtureA()->GetBody()->GetUserData());
		BodyUserData* userDataB = static_cast<BodyUserData*>(contact->GetFixtureB()->GetBody()->GetUserData());

		userDataA->collided = true;
		userDataB->collided = true;

		if (userDataA->bodyId == 102)
		{
			//if (userDataB->bodyId == 103)
			//	userDataA->bodyHealth -= (int)(impulse->normalImpulses[0] / 8);
			if (userDataB->bodyId != 102 && userDataB->bodyId != 103)
				userDataA->bodyHealth -= (int)(impulse->normalImpulses[0]);
		}
		else
		{
			userDataA->bodyHealth -= (int)impulse->normalImpulses[0];
		}
		if (userDataB->bodyId == 102)
		{
			//if (userDataA->bodyId == 103)
			//	userDataB->bodyHealth -= (int)(impulse->normalImpulses[0] / 8);
			if (userDataA->bodyId != 102 && userDataA->bodyId != 103)
				userDataB->bodyHealth -= (int)(impulse->normalImpulses[0]);
		}
		else
		{
			userDataB->bodyHealth -= (int)impulse->normalImpulses[0];
		}
	}
}