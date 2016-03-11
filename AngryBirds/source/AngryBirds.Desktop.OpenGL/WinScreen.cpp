#include "stdafx.h"
#include "WinScreen.h"


void WinScreen::init(char *imageSrc)
{
	WinScreenRenderer = NULL;
	loadText(imageSrc);
}

void WinScreen::render(int index)
{
	WinScreenRenderer->render();
}

void WinScreen::loadText(char* imageSrc)
{
	if (WinScreenRenderer != NULL)
	{
		delete WinScreenRenderer;
		WinScreenRenderer = NULL;
	}
	WinScreenRenderer = new Renderer();
	WinScreenRenderer->init(imageSrc, 1, 388, 566);

	WinScreenRenderer->setUTextureLeft(0, 0);
	WinScreenRenderer->setUTextureRight(0, 388);
	WinScreenRenderer->setVTextureTop(0, 0);
	WinScreenRenderer->setVTextureBottom(0, 566);

	WinScreenRenderer->setXPositionLeft(766);
	WinScreenRenderer->setXPositionRight(1154);
	WinScreenRenderer->setYPositionTop(257);
	WinScreenRenderer->setYPositionBottom(823);
}

WinScreen::~WinScreen()
{
	if (WinScreenRenderer != NULL)
	{
		delete WinScreenRenderer;
	}
}