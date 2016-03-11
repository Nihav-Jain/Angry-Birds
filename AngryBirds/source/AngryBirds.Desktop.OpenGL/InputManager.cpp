#include "stdafx.h"
#include "InputManager.h"

InputManagerC* InputManagerC::sInstance = NULL;

/**
 *	@method InputManagerC::InputManagerC
 *	@desc   constructor
 */
InputManagerC::InputManagerC()
{
	mMouseDown = false;
	mMouseUp = false;
	mMouseDownTimer = 0;
	mMouseClicked = false;
}

/**
*	@method InputManagerC::CreateInstance()
*	@desc   instantiates and returns the singleton instance
*/
InputManagerC *InputManagerC::CreateInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new InputManagerC();
	}
	return sInstance;
}

/**
 *	@method InputManagerC::init()
 *	@desc   inititlizes the input specific variables
 */
void InputManagerC::init()
{
	mMouseDown = false;
	mMouseUp = false;
	mMouseDownTimer = 0;
	mMouseClicked = false;
}

/**
 *	@method InputManagerC::update()
 *	@desc   called every frame, checks for input and updates member variables
 */
void InputManagerC::update(DWORD miliseconds)
{

	mMouseClicked = false;
	if (GetCursorPos(&this->mousePos))
	{

	}

	if (mouse_l_button_down)
	{
		mMouseDown = true;
		mMouseUp = false;

		mMouseDownTimer += miliseconds;
	}
	else
	{
		mMouseDown = false;
		mMouseUp = true;

		if (mMouseDownTimer > 0 && mMouseDownTimer <= MouseClickDuration)
		{
			mMouseClicked = true;
		}

		mMouseDownTimer = 0;
	}
}

/**
 *	@method InputManagerC::isMouseDown()
 *	@desc   getter for the mouse down state
 */
bool InputManagerC::isMouseDown()
{
	return mMouseDown;
}

/**
 *	@method InputManagerC::setMouseLButtonStatus()
 *	@desc   setter for the mouse button state
 */
void InputManagerC::setMouseLButtonStatus(bool status)
{
	mMouseDown = status;
}

/**
 *	@method InputManagerC::isMouseUp()
 *	@desc   getter for the mouse up state
 */
bool InputManagerC::isMouseUp()
{
	return mMouseUp;
}

/**
 *	@method InputManagerC::isMouseClicked()
 *	@desc   getter for the mouse clicked state
 */
bool InputManagerC::isMouseClicked()
{
	return mMouseClicked;
}

/**
 *	@method InputManagerC::getMouseX()
 *	@desc   getter for the mouse X coordinate
 */
float InputManagerC::getMouseX()
{
	return mousePos.x;
}

/**
 *	@method InputManagerC::getMouseY()
 *	@desc   getter for the mouse Y coordinate
 */
float InputManagerC::getMouseY()
{
	return CGame::mScreenHeight - mousePos.y;
}
