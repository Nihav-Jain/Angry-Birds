#ifndef LEVEL_H
#define LEVEL_H

#include "openGLFramework.h"
#include "SOIL.h"
#include "PhysicsWorld.h"
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glut.h>
#include "Box2D\Box2D.h"

/**
 *	@class Level
 *	@desc  manages level Box2D terrain
 */
class Level
{
	public:
		void init();
		void update(unsigned long millis);
		void render();
		~Level();
	private:
		b2Body *groundBody;
		BodyUserData groundUserData;
		GLuint mGroundTextureMap;
		float width;
		float height;
};

#endif