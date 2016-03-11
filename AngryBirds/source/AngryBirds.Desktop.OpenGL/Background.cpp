#include "stdafx.h"
#include "Background.h"

Background* Background::sInstance = NULL;

/**
 *	@method Background::createInstance()
 *	@desc   instantiates and returns the singleton instance
 */
Background* Background::createInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new Background();
	}
	return sInstance;
}

/**
 *	@method Background::createInstance()
 *	@desc   returns the singleton instance
 */
Background* Background::getInstance()
{
	return sInstance;
}


/**
 *	@method Background::init
 *	@desc   inititalizes the background image renderer
 */
void Background::init(const char *imageSrc)
{
	backgroundRenderer = NULL;
	loadBackground(imageSrc);
}

/**
 *	@method Background::render
 *	@desc   calls the background renderer
 */
void Background::render()
{
	backgroundRenderer->render();
}

/**
 *	@method Background::loadBackground
 *	@desc   loads the given texture and sets the UV coordinates for the renderer
 */
void Background::loadBackground(const char* imageSrc)
{
	if (backgroundRenderer != NULL)
	{
		delete backgroundRenderer;
		backgroundRenderer = NULL;
	}
	backgroundRenderer = new Renderer();
	backgroundRenderer->init(imageSrc, 1, CGame::mScreenWidth, CGame::mScreenHeight);

	backgroundRenderer->setUTextureLeft(0, 0);
	backgroundRenderer->setUTextureRight(0, CGame::mScreenWidth);
	backgroundRenderer->setVTextureTop(0, 0);
	backgroundRenderer->setVTextureBottom(0, CGame::mScreenHeight);

	backgroundRenderer->setXPositionLeft(0);
	backgroundRenderer->setXPositionRight(CGame::mScreenWidth);
	backgroundRenderer->setYPositionTop(0);
	backgroundRenderer->setYPositionBottom(CGame::mScreenHeight);
}

/**
*	@method Background::~Background
*	@desc   destructor to free up heap allocated memory
*/
Background::~Background()
{
	if (backgroundRenderer != NULL)
	{
		delete backgroundRenderer;
	}
}