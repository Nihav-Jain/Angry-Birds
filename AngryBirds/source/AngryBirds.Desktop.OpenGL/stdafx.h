// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

#include <tchar.h>
#include <windows.h>													// Header File For The Windows Library
#include <stdio.h>
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <cstdlib>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>

#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glut.h>
#include "SOIL.h"

#include "Box2D\Box2D.h"
#include "TinyXML\tinyxml.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#include "baseTypes.h"
#include "openGLFramework.h"
#include "HashMap.h"
#include "PhysicsWorld.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "game.h"