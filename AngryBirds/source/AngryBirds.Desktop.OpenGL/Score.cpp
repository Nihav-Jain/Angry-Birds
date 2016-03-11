#include "stdafx.h"
#include "Score.h"

void Score::init(TiXmlElement *scoreElement, int lives)
{
	float imageWidth, imageHeight;
	scoreElement->FirstChildElement()->QueryFloatAttribute("imageWidth", &imageWidth);
	scoreElement->FirstChildElement()->QueryFloatAttribute("imageHeight", &imageHeight);

	TextRenderer = new Renderer();
	TextRenderer->init(scoreElement->FirstChildElement()->Attribute("src"), 1, imageWidth, imageHeight);

	TextRenderer->setUTextureLeft(0, 0);
	TextRenderer->setUTextureRight(0, imageWidth);
	TextRenderer->setVTextureTop(0, 0);
	TextRenderer->setVTextureBottom(0, imageHeight);

	TextRenderer->setXPositionLeft(0);
	TextRenderer->setXPositionRight(imageWidth);
	TextRenderer->setYPositionTop(0);
	TextRenderer->setYPositionBottom(imageHeight);

	score = lives;

	TiXmlElement *fontElement = scoreElement->FirstChildElement()->NextSiblingElement();
	fontElement->QueryFloatAttribute("imageWidth", &imageWidth);
	fontElement->QueryFloatAttribute("imageHeight", &imageHeight);

	int fontGlyphCount = 0;
	TiXmlElement *glyphElement = fontElement->FirstChildElement();
	while (glyphElement != NULL)
	{
		fontGlyphCount++;
		glyphElement = glyphElement->NextSiblingElement();
	}

	ScoreRenderer = new Renderer();
	ScoreRenderer->init(fontElement->Attribute("src"), fontGlyphCount, imageWidth, imageHeight);

	glyphElement = fontElement->FirstChildElement();
	float uPosLeft, uPosRight, vPosTop, vPosBottom;
	for (int i = 0; i < fontGlyphCount; i++)
	{
		glyphElement->QueryFloatAttribute("uLeft", &uPosLeft);
		glyphElement->QueryFloatAttribute("uRight", &uPosRight);
		glyphElement->QueryFloatAttribute("vTop", &vPosTop);
		glyphElement->QueryFloatAttribute("vBottom", &vPosBottom);

		ScoreRenderer->setUTextureLeft(i, uPosLeft);
		ScoreRenderer->setUTextureRight(i, uPosRight);
		ScoreRenderer->setVTextureTop(i, vPosTop);
		ScoreRenderer->setVTextureBottom(i, vPosBottom);

		glyphElement = glyphElement->NextSiblingElement();
	}

	float screenX, screenY, width, height;
	scoreElement->QueryFloatAttribute("screenX", &screenX);
	scoreElement->QueryFloatAttribute("screenY", &screenY);
	scoreElement->QueryFloatAttribute("width", &width);
	scoreElement->QueryFloatAttribute("height", &height);

	ScoreRenderer->setXPositionLeft(screenX);
	ScoreRenderer->setXPositionRight(screenX + width);
	ScoreRenderer->setYPositionTop(screenY);
	ScoreRenderer->setYPositionBottom(screenY + height);
}


void Score::render()
{
	TextRenderer->render();
	ScoreRenderer->render(score);
}

void Score::SetScore(int lives)
{
	score = lives;
}

Score::~Score()
{
	delete TextRenderer;
	delete ScoreRenderer;
}