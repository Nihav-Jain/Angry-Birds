#include "stdafx.h"
#include "Poof.h"

Poof* Poof::sInstance = NULL;

Poof* Poof::createInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new Poof();
	}
	return sInstance;
}

Poof* Poof::getInstance()
{
	return sInstance;
}

void Poof::init(TiXmlElement* poofElement)
{
	int totalPoofFrames = 0;
	TiXmlElement* poofSprite = poofElement->FirstChildElement()->FirstChildElement();
	while (poofSprite != NULL)
	{
		poofSprite = poofSprite->NextSiblingElement();
		totalPoofFrames++;
	}

	poofData.numSprites = totalPoofFrames;
	poofData.spriteData = (SpriteStateData *)malloc(poofData.numSprites * sizeof(SpriteStateData));

	int j = 0;
	poofSprite = poofElement->FirstChildElement()->FirstChildElement();
	while (poofSprite != NULL)
	{
		poofData.spriteData[j].spriteState = poofSprite->Attribute("state");
		poofSprite->QueryFloatAttribute("xPosLeft", &poofData.spriteData[j].uLeft);
		poofSprite->QueryFloatAttribute("xPosRight", &poofData.spriteData[j].uRight);
		poofSprite->QueryFloatAttribute("yPosTop", &poofData.spriteData[j].vTop);
		poofSprite->QueryFloatAttribute("yPosBottom", &poofData.spriteData[j].vBottom);

		poofSprite = poofSprite->NextSiblingElement();
		j++;
	}

	float imageWidth, imageHeight;
	poofElement->QueryFloatAttribute("imageWidth", &imageWidth);
	poofElement->QueryFloatAttribute("imageHeight", &imageHeight);

	poofRenderer = new Renderer();
	poofRenderer->init(poofElement->Attribute("src"), totalPoofFrames, imageWidth, imageHeight);

	int k = 0;

	for (j = 0; j < poofData.numSprites; j++)
	{
		poofRenderer->setUTextureLeft(k, poofData.spriteData[j].uLeft);
		poofRenderer->setUTextureRight(k, poofData.spriteData[j].uRight);
		poofRenderer->setVTextureTop(k, poofData.spriteData[j].vTop);
		poofRenderer->setVTextureBottom(k, poofData.spriteData[j].vBottom);

		poofData.spriteData[j].rendererIndex = k;

		k++;
	}
}

void Poof::update(unsigned long miliseconds)
{

}

void Poof::render(int renderIndex)
{

}