#include "stdafx.h"
#include "AngryBirdsMouse.h"

AngryBirdsMouse* AngryBirdsMouse::sInstance = NULL;

/**
 *	@method AngryBirdsMouse::getInstance
 *	@desc   returns the static singleton instance, instantiates it first if it is null, 
 */
AngryBirdsMouse* AngryBirdsMouse::getInstance()
{
	if (AngryBirdsMouse::sInstance == NULL)
	{
		sInstance = new AngryBirdsMouse();
	}
	return sInstance;
}

/**
 *	@method AngryBirdsMouse::init
 *	@desc   initializes the mouse cursor renderer with the texture and UN coordinates for each state
 */
void AngryBirdsMouse::init(TiXmlElement* mouseElement)
{
	float imageWidth, imageHeight;
	mouseElement->QueryFloatAttribute("imageWidth", &imageWidth);
	mouseElement->QueryFloatAttribute("imageHeight", &imageHeight);

	mouseElement->QueryIntAttribute("cursorWidth", &cursorWidth);
	mouseElement->QueryIntAttribute("cursorHeight", &cursorHeight);

	TiXmlElement *cursorElement = mouseElement->FirstChildElement();
	int mouseCursorCount = 0;
	while (cursorElement != NULL)
	{
		mouseCursorCount++;
		cursorElement = cursorElement->NextSiblingElement();
	}

	mouseRenderer = new Renderer();
	mouseRenderer->init(mouseElement->Attribute("src"), mouseCursorCount, imageWidth, imageHeight);
	cursorElement = mouseElement->FirstChildElement();
	for (int i = 0; i < mouseCursorCount; i++)
	{
		float uLeft, uRight, vTop, vBottom;
		cursorElement->QueryFloatAttribute("uLeft", &uLeft);
		cursorElement->QueryFloatAttribute("uRight", &uRight);
		cursorElement->QueryFloatAttribute("vTop", &vTop);
		cursorElement->QueryFloatAttribute("vBottom", &vBottom);

		mouseRenderer->setUTextureLeft(i, uLeft);
		mouseRenderer->setUTextureRight(i, uRight);
		mouseRenderer->setVTextureTop(i, vTop);
		mouseRenderer->setVTextureBottom(i, vBottom);

		cursorElement = cursorElement->NextSiblingElement();
	}

	renderIndex = 0;
}

/**
 *	@method AngryBirdsMouse::update
 *	@desc   called every frame, updates the mouse cursor position and sprite
 */
void AngryBirdsMouse::update()
{
	float mousex = InputManagerC::GetInstance()->getMouseX();
	float mousey = InputManagerC::GetInstance()->getMouseY();

	// set screen pos
	mouseRenderer->setXPositionLeft(mousex);
	mouseRenderer->setXPositionRight(mousex + cursorWidth);
	mouseRenderer->setYPositionTop(mousey - cursorHeight);
	mouseRenderer->setYPositionBottom(mousey);

	renderIndex = 0;

	if (CGame::mGameState == START_MENU)
	{
		if (InputManagerC::GetInstance()->isMouseDown())
		{
			renderIndex = 1;
		}
		else
		{
			renderIndex = 0;
		}
	}
	else if (CGame::mGameState == LEVEL)
	{
		if (InputManagerC::GetInstance()->isMouseDown())
		{
			renderIndex = 3;
		}
		else
		{
			renderIndex = 2;
		}

	}
}

/**
 *	@method AngryBirdsMouse::getInstance
 *	@desc   calls the renderer of the mouse cursor
 */
void AngryBirdsMouse::render()
{
	mouseRenderer->render(renderIndex);
}

/**
 *	@method AngryBirdsMouse::~AngryBirdsMouse
 *	@desc   destructor to free up heap allocated memory
 */
AngryBirdsMouse::~AngryBirdsMouse()
{
	delete mouseRenderer;
}