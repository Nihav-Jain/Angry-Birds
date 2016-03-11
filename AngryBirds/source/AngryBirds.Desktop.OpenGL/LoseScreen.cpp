#include "stdafx.h"
#include "LoseScreen.h"


void LoseScreen::init(char *imageSrc)
{
	LoseScreenRenderer = NULL;
	loadText(imageSrc);
}

void LoseScreen::render(int index)
{
	LoseScreenRenderer->render();
}

void LoseScreen::loadText(char* imageSrc)
{
	if (LoseScreenRenderer != NULL)
	{
		delete LoseScreenRenderer;
		LoseScreenRenderer = NULL;
	}
	LoseScreenRenderer = new Renderer();
	LoseScreenRenderer->init(imageSrc, 1, 388, 566);

	LoseScreenRenderer->setUTextureLeft(0, 0);
	LoseScreenRenderer->setUTextureRight(0, 388);
	LoseScreenRenderer->setVTextureTop(0, 0);
	LoseScreenRenderer->setVTextureBottom(0, 566);

	LoseScreenRenderer->setXPositionLeft(766);
	LoseScreenRenderer->setXPositionRight(1154);
	LoseScreenRenderer->setYPositionTop(257);
	LoseScreenRenderer->setYPositionBottom(823);
}

LoseScreen::~LoseScreen()
{
	if (LoseScreenRenderer != NULL)
	{
		delete LoseScreenRenderer;
	}
}