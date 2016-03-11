#include "stdafx.h"
#include "BlockManager.h"


BlockManager* BlockManager::sInstance = NULL;

/**
 *	@method BlockManager::createInstance()
 *	@desc   instantiates and returns the singleton instance of the BlockManager
 */
BlockManager* BlockManager::createInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new BlockManager();
	}
	return sInstance;
}

/**
 *	@method BlockManager::getInstance()
 *	@desc   returns the singleton instance of the BlockManager
 */
BlockManager* BlockManager::getInstance()
{
	return sInstance;
}

/**
 *	@method BlockManager::init()
 *	@desc   parses the xml to store the properties of all possible block types like the UV coordinates of the sprite states and physics properties
 */
void BlockManager::init(TiXmlElement* blocksElement)
{
	totalPossibleBlocks = 0;
	int totalSpriteCount = 0;

	TiXmlElement* blockElement = blocksElement->FirstChildElement();
	while (blockElement != NULL)
	{
		totalPossibleBlocks++;
		blockElement = blockElement->NextSiblingElement();
	}

	allBlocksData = (ObjectData *)malloc(totalPossibleBlocks * sizeof(ObjectData));
	blockElement = blocksElement->FirstChildElement();
	int i = 0, j = 0;
	while (blockElement != NULL)
	{
		allBlocksData[i].objectType = blockElement->Attribute("type");

		TiXmlElement* physicsElement = blockElement->FirstChildElement();
		physicsElement->QueryFloatAttribute("density", &allBlocksData[i].density);
		physicsElement->QueryFloatAttribute("restitution", &allBlocksData[i].restitution);
		physicsElement->QueryFloatAttribute("friction", &allBlocksData[i].friction);
		physicsElement->QueryIntAttribute("health", &allBlocksData[i].health);

		TiXmlElement* spritesParentElement = physicsElement->NextSiblingElement();
		int spriteStateCount = 0;
		TiXmlElement* spriteElement = spritesParentElement->FirstChildElement();
		while (spriteElement != NULL)
		{
			spriteStateCount++;
			spriteElement = spriteElement->NextSiblingElement();
		}

		totalSpriteCount += spriteStateCount;

		allBlocksData[i].spriteData = (SpriteStateData *)malloc(spriteStateCount * sizeof(SpriteStateData));
		allBlocksData[i].numSprites = spriteStateCount;
		j = 0;
		spriteElement = spritesParentElement->FirstChildElement();
		while (spriteElement != NULL)
		{
			allBlocksData[i].spriteData[j].spriteState = spriteElement->Attribute("state");
			spriteElement->QueryFloatAttribute("xPosLeft", &allBlocksData[i].spriteData[j].uLeft);
			spriteElement->QueryFloatAttribute("xPosRight", &allBlocksData[i].spriteData[j].uRight);
			spriteElement->QueryFloatAttribute("yPosTop", &allBlocksData[i].spriteData[j].vTop);
			spriteElement->QueryFloatAttribute("yPosBottom", &allBlocksData[i].spriteData[j].vBottom);

			spriteElement = spriteElement->NextSiblingElement();
			j++;
		}

		blockElement = blockElement->NextSiblingElement();
		i++;
	}

	float imageWidth, imageHeight;
	blocksElement->QueryFloatAttribute("imageWidth", &imageWidth);
	blocksElement->QueryFloatAttribute("imageHeight", &imageHeight);

	blockRenderer = new Renderer();
	blockRenderer->init(blocksElement->Attribute("src"), totalSpriteCount, imageWidth, imageHeight);

	int k = 0;

	for (i = 0; i < totalPossibleBlocks; i++)
	{
		for (j = 0; j < allBlocksData[i].numSprites; j++)
		{
			blockRenderer->setUTextureLeft(k, allBlocksData[i].spriteData[j].uLeft);
			blockRenderer->setUTextureRight(k, allBlocksData[i].spriteData[j].uRight);
			blockRenderer->setVTextureTop(k, allBlocksData[i].spriteData[j].vTop);
			blockRenderer->setVTextureBottom(k, allBlocksData[i].spriteData[j].vBottom);

			allBlocksData[i].spriteData[j].rendererIndex = k;

			k++;
		}
	}

}

/**
 *	@method BlockManager::loadLevel()
 *	@desc   parse the current level xml and instantiate the blocks in it
 */
void BlockManager::loadLevel(TiXmlElement* levelBlockElement)
{
	if (numBlocks > 0)
	{
		shutdown();
	}

	numBlocks = 0;
	TiXmlElement* levelBlock = levelBlockElement->FirstChildElement();
	while (levelBlock != NULL)
	{
		numBlocks++;
		levelBlock = levelBlock->NextSiblingElement();
	}

	blockList = new WoodBlock*[numBlocks];
	int i = 0;
	int x, y;
	float wid, ht, angle;

	levelBlock = levelBlockElement->FirstChildElement();
	ObjectData currentBlockData;

	while (levelBlock != NULL)
	{
		const char* currentBlockType = levelBlock->Attribute("type");
		int j;
		for (j = 0; j < totalPossibleBlocks; j++)
		{
			if (strcmp(currentBlockType, allBlocksData[j].objectType) == 0)
			{
				currentBlockData = allBlocksData[j];
				break;
			}
		}
		assert(j < totalPossibleBlocks);

		levelBlock->QueryIntAttribute("x", &x);
		levelBlock->QueryIntAttribute("y", &y);
		levelBlock->QueryFloatAttribute("wid", &wid);
		levelBlock->QueryFloatAttribute("ht", &ht);
		levelBlock->QueryFloatAttribute("angle", &angle);

		blockList[i] = new WoodBlock();
		blockList[i]->init(x, y, wid, ht, angle, currentBlockData);

		levelBlock = levelBlock->NextSiblingElement();
		i++;
	}
}

/**
 *	@method BlockManager::update()
 *	@desc   called every frame (currently unused)
 */
void BlockManager::update()
{
	int i;
	for (i = 0; i < numBlocks; i++)
	{
		blockList[i]->update();
	}
}

/**
 *	@method BlockManager::render()
 *	@desc   renders each block currently in the level
 */
void BlockManager::render()
{
	int i;
	for (i = 0; i < numBlocks; i++)
	{
		if (blockList[i]->isBlockAlive())
		{
			b2Vec2 position = blockList[i]->getBodyPosition();
			float32 angle = blockList[i]->getBodyAngleInRads();

			float width = blockList[i]->getBodyWidth();
			float height = blockList[i]->getBodyHeight();

			float xPositionLeft = (position.x * PhysicsWorld::RATIO) - width / 2;
			float xPositionRight = xPositionLeft + width;

			float yPositionTop = ((position.y * PhysicsWorld::RATIO) + height / 2);
			float yPositionBottom = yPositionTop + height;

			blockRenderer->setXPositionLeft(xPositionLeft);
			blockRenderer->setXPositionRight(xPositionRight);

			blockRenderer->setYPositionTop(yPositionTop);
			blockRenderer->setYPositionBottom(yPositionBottom);

			int blockSpriteIndex = blockList[i]->getBlockRenderIndex();
			if (blockList[i]->getBlockHealth() < 0.75 * blockList[i]->getBlockMaxHealth() && blockList[i]->getBlockHealth() > 0.50 * blockList[i]->getBlockMaxHealth())
			{
				blockSpriteIndex += 1;
			}
			else if (blockList[i]->getBlockHealth() < 0.50 * blockList[i]->getBlockMaxHealth())
			{
				blockSpriteIndex += 2;
			}

			blockRenderer->render(blockSpriteIndex, angle * 180 / 3.14159265, height, width);
		}
	}
}

/**
 *	@method BlockManager::shutdown()
 *	@desc   frees up the heap memory allocated to store current level blocks
 */
void BlockManager::shutdown()
{
	int i;
	for (i = 0; i < numBlocks; i++)
	{
		blockList[i]->shutdown();
		delete blockList[i];
	}
	delete[] blockList;
}

/**
 *	@method BlockManager::~BlockManager()
 *	@desc   destructor to free up the heap memory allocated
 */
BlockManager::~BlockManager()
{
	shutdown();
	delete blockRenderer;

	int i;
	for (i = 0; i < totalPossibleBlocks; i++)
	{
		delete allBlocksData[i].spriteData;
	}

	delete allBlocksData;	// delete sprite states individually
}