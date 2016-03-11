#include "stdafx.h"
#include "PigManager.h"

PigManager* PigManager::sInstance = NULL;

/**
 *	@method PigManager::createInstance()
 *	@desc   instantiates and returns the singleton instance
 */
PigManager* PigManager::createInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new PigManager();
	}
	return sInstance;
}

/**
 *	@method PigManager::getInstance()
 *	@desc   returns the singleton instance
 */
PigManager* PigManager::getInstance()
{
	return sInstance;
}

/**
 *	@method PigManager::init()
 *	@desc   initializes the pig renderer and stores the properties of all possible types of pigs like the UV coordinates of the sprite states and physics properties
 */
void PigManager::init(TiXmlElement* pigsElement, TiXmlElement* poofElement)
{
	numPigs = 0;
	totalPossiblePigs = 0;
	int totalSpriteCount = 0;

	TiXmlElement* pigElement = pigsElement->FirstChildElement();
	while (pigElement != NULL)
	{
		totalPossiblePigs++;
		pigElement = pigElement->NextSiblingElement();
	}

	allPigsData = (ObjectData *)malloc(totalPossiblePigs * sizeof(ObjectData));
	pigElement = pigsElement->FirstChildElement();
	int i = 0, j = 0;
	while (pigElement != NULL)
	{
		allPigsData[i].objectType = pigElement->Attribute("type");

		TiXmlElement* physicsElement = pigElement->FirstChildElement();
		physicsElement->QueryFloatAttribute("density", &allPigsData[i].density);
		physicsElement->QueryFloatAttribute("restitution", &allPigsData[i].restitution);
		physicsElement->QueryFloatAttribute("friction", &allPigsData[i].friction);
		physicsElement->QueryIntAttribute("health", &allPigsData[i].health);

		TiXmlElement* spritesParentElement = physicsElement->NextSiblingElement();
		int spriteStateCount = 0;
		TiXmlElement* spriteElement = spritesParentElement->FirstChildElement();
		while (spriteElement != NULL)
		{
			spriteStateCount++;
			spriteElement = spriteElement->NextSiblingElement();
		}

		totalSpriteCount += spriteStateCount;

		allPigsData[i].spriteData = (SpriteStateData *)malloc(spriteStateCount * sizeof(SpriteStateData));
		allPigsData[i].numSprites = spriteStateCount;
		j = 0;
		spriteElement = spritesParentElement->FirstChildElement();
		while (spriteElement != NULL)
		{
			allPigsData[i].spriteData[j].spriteState = spriteElement->Attribute("state");
			spriteElement->QueryFloatAttribute("xPosLeft", &allPigsData[i].spriteData[j].uLeft);
			spriteElement->QueryFloatAttribute("xPosRight", &allPigsData[i].spriteData[j].uRight);
			spriteElement->QueryFloatAttribute("yPosTop", &allPigsData[i].spriteData[j].vTop);
			spriteElement->QueryFloatAttribute("yPosBottom", &allPigsData[i].spriteData[j].vBottom);

			spriteElement = spriteElement->NextSiblingElement();
			j++;
		}

		pigElement = pigElement->NextSiblingElement();
		i++;
	}

	/**************************/

	int totalPoofFrames = 0;
	TiXmlElement* poofSprite = poofElement->FirstChildElement()->FirstChildElement();
	while (poofSprite != NULL)
	{
		poofSprite = poofSprite->NextSiblingElement();
		totalPoofFrames++;
	}

	poofData.numSprites = totalPoofFrames;
	poofData.spriteData = (SpriteStateData *)malloc(poofData.numSprites * sizeof(SpriteStateData));

	j = 0;
	poofSprite = poofElement->FirstChildElement()->FirstChildElement();
	while (poofSprite != NULL)
	{
		poofData.spriteData[j].spriteState = poofSprite->Attribute("state");
		poofSprite->QueryFloatAttribute("xPosLeft", &poofData.spriteData[j].uLeft);
		poofSprite->QueryFloatAttribute("xPosRight", &poofData.spriteData[j].uRight);
		poofSprite->QueryFloatAttribute("yPosTop", &poofData.spriteData[j].vTop);
		poofSprite->QueryFloatAttribute("yPosBottom", &poofData.spriteData[j].vBottom);

		//pigRenderer->setUTextureLeft(k, poofData.spriteData[j].uLeft);
		//pigRenderer->setUTextureRight(k, poofData.spriteData[j].uRight);
		//pigRenderer->setVTextureTop(k, poofData.spriteData[j].vTop);
		//pigRenderer->setVTextureBottom(k, poofData.spriteData[j].vBottom);

		//poofData.spriteData[j].rendererIndex = k;

		//k++;

		poofSprite = poofSprite->NextSiblingElement();
		j++;
	}

	totalSpriteCount += totalPoofFrames;
	
	/*************************/

	float imageWidth, imageHeight;
	pigsElement->QueryFloatAttribute("imageWidth", &imageWidth);
	pigsElement->QueryFloatAttribute("imageHeight", &imageHeight);

	pigRenderer = new Renderer();
	pigRenderer->init(pigsElement->Attribute("src"), totalSpriteCount, imageWidth, imageHeight);

	int k = 0;

	for (i = 0; i < totalPossiblePigs; i++)
	{
		for (j = 0; j < allPigsData[i].numSprites; j++)
		{
			pigRenderer->setUTextureLeft(k, allPigsData[i].spriteData[j].uLeft);
			pigRenderer->setUTextureRight(k, allPigsData[i].spriteData[j].uRight);
			pigRenderer->setVTextureTop(k, allPigsData[i].spriteData[j].vTop);
			pigRenderer->setVTextureBottom(k, allPigsData[i].spriteData[j].vBottom);

			allPigsData[i].spriteData[j].rendererIndex = k;

			k++;
		}
	}

	/********Poofing*********/
	j = 0;
	poofSprite = poofElement->FirstChildElement()->FirstChildElement();
	while (poofSprite != NULL)
	{
		pigRenderer->setUTextureLeft(k, poofData.spriteData[j].uLeft);
		pigRenderer->setUTextureRight(k, poofData.spriteData[j].uRight);
		pigRenderer->setVTextureTop(k, poofData.spriteData[j].vTop);
		pigRenderer->setVTextureBottom(k, poofData.spriteData[j].vBottom);

		poofData.spriteData[j].rendererIndex = k;

		k++;

		poofSprite = poofSprite->NextSiblingElement();
		j++;
	}

}

/**
 *	@method PigManager::loadLevel()
 *	@desc   parse the current level xml and instantiate the pigs in it
 */
void PigManager::loadLevel(TiXmlElement* levelPigElement)
{
	if (numPigs > 0)
	{
		shutdown();
	}

	numPigs = 0;
	TiXmlElement* levelPig = levelPigElement->FirstChildElement();
	while (levelPig != NULL)
	{
		numPigs++;
		levelPig = levelPig->NextSiblingElement();
	}

	pigList = new EnemyPig*[numPigs];
	int i = 0;
	int x, y;
	float wid, ht;

	levelPig = levelPigElement->FirstChildElement();
	ObjectData currentPigData;

	while (levelPig != NULL)
	{
		const char* currentBlockType = levelPig->Attribute("type");
		int j;
		for (j = 0; j < totalPossiblePigs; j++)
		{
			if (strcmp(currentBlockType, allPigsData[j].objectType) == 0)
			{
				currentPigData = allPigsData[j];
				break;
			}
		}
		assert(j < totalPossiblePigs);

		levelPig->QueryIntAttribute("x", &x);
		levelPig->QueryIntAttribute("y", &y);
		levelPig->QueryFloatAttribute("wid", &wid);
		levelPig->QueryFloatAttribute("ht", &ht);

		pigList[i] = new EnemyPig();
		pigList[i]->init(x, y, wid, currentPigData);

		levelPig = levelPig->NextSiblingElement();
		i++;
	}

	CGame::GetInstance()->setPigCount(numPigs);

}

/**
 *	@method PigManager::loadLevel()
 *	@desc   called every frame, updates the pigs in the current level
 */
void PigManager::update(DWORD milliseconds)
{
	int i;
	for (i = 0; i < numPigs; i++)
	{
		pigList[i]->update();
	}

	deltaTime = milliseconds;

}

/**
 *	@method PigManager::render()
 *	@desc   calls the renderer of the pigs which are still alive
 */
void PigManager::render()
{
	int i;
	for (i = 0; i < numPigs; i++)
	{
		if (pigList[i]->isPigAlive())
		{
			b2Vec2 position = pigList[i]->getPigBodyPosition();
			float32 angle = pigList[i]->getPigBodyAngleInRads();

			float width = pigList[i]->getPigBodyWidth();

			float xPositionLeft = (position.x * PhysicsWorld::RATIO) - width / 2;
			float xPositionRight = xPositionLeft + width;

			float yPositionTop = ((position.y * PhysicsWorld::RATIO) - width / 4);
			float yPositionBottom = yPositionTop + width;

			pigRenderer->setXPositionLeft(xPositionLeft);
			pigRenderer->setXPositionRight(xPositionRight);

			pigRenderer->setYPositionTop(yPositionTop);
			pigRenderer->setYPositionBottom(yPositionBottom);

			int pigSpriteIndex = pigList[i]->getPigRenderIndex();
			if (pigList[i]->getPigHealth() < 0.75 * pigList[i]->getPigMaxHealth() && pigList[i]->getPigHealth() > 0.50 * pigList[i]->getPigMaxHealth())
			{
				pigSpriteIndex += 1;
			}
			else if (pigList[i]->getPigHealth() < 0.50 * pigList[i]->getPigMaxHealth())
			{
				pigSpriteIndex += 2;
			}

			pigRenderer->render(pigSpriteIndex, angle * 180 / 3.14159265, width, width);

		}
		else
		{
			if (pigList[i]->getPoofAnimationFrame() < poofData.numSprites)
			{

				int frame = pigList[i]->getPoofAnimationFrame();

				float poofX = pigList[i]->getPoofX();
				float poofY = pigList[i]->getPoofY();

				float poofFrameWidth = poofData.spriteData[frame].uRight - poofData.spriteData[frame].uLeft;
				float poofFrameHeight = poofData.spriteData[frame].vBottom - poofData.spriteData[frame].vTop;

				pigRenderer->setXPositionLeft(poofX - poofFrameWidth / 2);
				pigRenderer->setXPositionRight(poofX + poofFrameWidth / 2);

				pigRenderer->setYPositionTop(poofY - poofFrameHeight / 2);
				pigRenderer->setYPositionBottom(poofY + poofFrameHeight / 2);

				pigRenderer->render(poofData.spriteData[frame].rendererIndex);

				if (pigList[i]->getPoofAnimationDeltaTime() > 41)
				{
					pigList[i]->setPoofAnimationFrame(frame + 1);
					pigList[i]->setPoofAnimationDeltaTime(0);
				}
				else
				{
					pigList[i]->setPoofAnimationDeltaTime(pigList[i]->getPoofAnimationDeltaTime() + deltaTime);
				}

			}
		}
	}

}

//void PigManager::startPoofing(float px, float py)
//{
//	poofing = true;
//	poofAnimationFrame = 0;
//
//	poofX = px;
//	poofY = py;
//}

void PigManager::poof()
{

}

/**
 *	@method PigManager::shutdown()
 *	@desc   frees up heap allocated list of pigs of current level
 */
 void PigManager::shutdown()
{
	int i;
	for (i = 0; i < numPigs; i++)
	{
		pigList[i]->shutdown();
		delete pigList[i];
	}
	delete[] pigList;
}

 /**
  *	@method PigManager::~PigManager()
  *	@desc   destuctor to free to heap allocations
  */
 PigManager::~PigManager()
{
	shutdown();
	delete pigRenderer;

	int i;
	for (i = 0; i < totalPossiblePigs; i++)
	{
		delete allPigsData[i].spriteData;
	}
	delete allPigsData;
}