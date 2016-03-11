#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager* PlayerManager::sInstance = NULL;

/**
 *	@method PlayerManager::createInstance()
 *	@desc   instantiates and returns the singleton instances
 */
PlayerManager* PlayerManager::createInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new PlayerManager();
	}
	return sInstance;
}

/**
 *	@method PlayerManager::getInstance()
 *	@desc   returns the singleton instances
 */
PlayerManager* PlayerManager::getInstance()
{
	return sInstance;
}

/**
 *	@method PlayerManager::init()
 *	@desc   initializes the renderer and stores properties of all possible types of birds
 */
void PlayerManager::init(TiXmlElement* birdsElement, TiXmlElement* catapultElement, TiXmlElement* poofElement)
{

	totalPossibleBirds = 0;
	int totalSpriteCount = 3;	// the first 2 sprites are reserved for the catapult the catapult

	TiXmlElement* birdElement = birdsElement->FirstChildElement();
	while (birdElement != NULL)
	{
		totalPossibleBirds++;
		birdElement = birdElement->NextSiblingElement();
	}

	allBirdsData = (ObjectData *)malloc(totalPossibleBirds * sizeof(ObjectData));
	int i = 0, j = 0;
	birdElement = birdsElement->FirstChildElement();
	while (birdElement != NULL)
	{
		allBirdsData[i].objectType = birdElement->Attribute("type");

		TiXmlElement* physicsElement = birdElement->FirstChildElement();
		physicsElement->QueryFloatAttribute("density", &allBirdsData[i].density);
		physicsElement->QueryFloatAttribute("restitution", &allBirdsData[i].restitution);
		physicsElement->QueryFloatAttribute("friction", &allBirdsData[i].friction);
		physicsElement->QueryIntAttribute("health", &allBirdsData[i].health);

		TiXmlElement* spritesParentElement = physicsElement->NextSiblingElement();
		int spriteStateCount = 0;
		TiXmlElement* spriteElement = spritesParentElement->FirstChildElement();
		while (spriteElement != NULL)
		{
			spriteStateCount++;
			spriteElement = spriteElement->NextSiblingElement();
		}

		totalSpriteCount += spriteStateCount;

		allBirdsData[i].spriteData = (SpriteStateData *)malloc(spriteStateCount * sizeof(SpriteStateData));
		allBirdsData[i].numSprites = spriteStateCount;
		j = 0;
		spriteElement = spritesParentElement->FirstChildElement();
		while (spriteElement != NULL)
		{
			allBirdsData[i].spriteData[j].spriteState = spriteElement->Attribute("state");
			spriteElement->QueryFloatAttribute("xPosLeft", &allBirdsData[i].spriteData[j].uLeft);
			spriteElement->QueryFloatAttribute("xPosRight", &allBirdsData[i].spriteData[j].uRight);
			spriteElement->QueryFloatAttribute("yPosTop", &allBirdsData[i].spriteData[j].vTop);
			spriteElement->QueryFloatAttribute("yPosBottom", &allBirdsData[i].spriteData[j].vBottom);

			spriteElement = spriteElement->NextSiblingElement();
			j++;
		}


		birdElement = birdElement->NextSiblingElement();
		i++;
	}

	/*******************************************************/

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

		poofSprite = poofSprite->NextSiblingElement();
		j++;
	}

	totalSpriteCount += totalPoofFrames;

	/*****************************************************/

	float imageWidth, imageHeight;
	birdsElement->QueryFloatAttribute("imageWidth", &imageWidth);
	birdsElement->QueryFloatAttribute("imageHeight", &imageHeight);

	playerRenderer = new Renderer();
	playerRenderer->init(birdsElement->Attribute("src"), totalSpriteCount, imageWidth, imageHeight);

	int k = 0;
	
	float uLeft, uRight, vTop, vBottom;
	
	TiXmlElement* catapult = catapultElement->FirstChildElement();
	catapult->QueryFloatAttribute("uLeft", &uLeft);
	catapult->QueryFloatAttribute("uRight", &uRight);
	catapult->QueryFloatAttribute("vTop", &vTop);
	catapult->QueryFloatAttribute("vBottom", &vBottom);

	playerRenderer->setUTextureLeft(k, uLeft);
	playerRenderer->setUTextureRight(k, uRight);
	playerRenderer->setVTextureTop(k, vTop);
	playerRenderer->setVTextureBottom(k, vBottom);

	catapult->QueryFloatAttribute("screenX", &rCatapultXLeft);
	catapult->QueryFloatAttribute("wid", &rCatapultXRight);
	catapult->QueryFloatAttribute("screenY", &rCatapultYTop);
	catapult->QueryFloatAttribute("ht", &rCatapultYBottom);

	rCatapultXRight += rCatapultXLeft;
	rCatapultYBottom += rCatapultYTop;

	k++;

	catapult = catapult->NextSiblingElement();
	catapult->QueryFloatAttribute("uLeft", &uLeft);
	catapult->QueryFloatAttribute("uRight", &uRight);
	catapult->QueryFloatAttribute("vTop", &vTop);
	catapult->QueryFloatAttribute("vBottom", &vBottom);

	playerRenderer->setUTextureLeft(k, uLeft);
	playerRenderer->setUTextureRight(k, uRight);
	playerRenderer->setVTextureTop(k, vTop);
	playerRenderer->setVTextureBottom(k, vBottom);

	catapult->QueryFloatAttribute("screenX", &lCatapultXLeft);
	catapult->QueryFloatAttribute("wid", &lCatapultXRight);
	catapult->QueryFloatAttribute("screenY", &lCatapultYTop);
	catapult->QueryFloatAttribute("ht", &lCatapultYBottom);

	lCatapultXRight += lCatapultXLeft;
	lCatapultYBottom += lCatapultYTop;

	k++;

	catapult = catapult->NextSiblingElement();
	catapult->QueryFloatAttribute("uLeft", &uLeft);
	catapult->QueryFloatAttribute("uRight", &uRight);
	catapult->QueryFloatAttribute("vTop", &vTop);
	catapult->QueryFloatAttribute("vBottom", &vBottom);

	playerRenderer->setUTextureLeft(k, uLeft);
	playerRenderer->setUTextureRight(k, uRight);
	playerRenderer->setVTextureTop(k, vTop);
	playerRenderer->setVTextureBottom(k, vBottom);

	k++;

	for (i = 0; i < totalPossibleBirds; i++)
	{
		for (j = 0; j < allBirdsData[i].numSprites; j++)
		{
			playerRenderer->setUTextureLeft(k, allBirdsData[i].spriteData[j].uLeft);
			playerRenderer->setUTextureRight(k, allBirdsData[i].spriteData[j].uRight);
			playerRenderer->setVTextureTop(k, allBirdsData[i].spriteData[j].vTop);
			playerRenderer->setVTextureBottom(k, allBirdsData[i].spriteData[j].vBottom);

			allBirdsData[i].spriteData[j].rendererIndex = k;

			k++;
		}
	}

	j = 0;
	poofSprite = poofElement->FirstChildElement()->FirstChildElement();
	while (poofSprite != NULL)
	{
		playerRenderer->setUTextureLeft(k, poofData.spriteData[j].uLeft);
		playerRenderer->setUTextureRight(k, poofData.spriteData[j].uRight);
		playerRenderer->setVTextureTop(k, poofData.spriteData[j].vTop);
		playerRenderer->setVTextureBottom(k, poofData.spriteData[j].vBottom);

		poofData.spriteData[j].rendererIndex = k;

		k++;

		poofSprite = poofSprite->NextSiblingElement();
		j++;
	}


}

/**
 *	@method PlayerManager::loadLevel()
 *	@desc   initializes the birds for the current level
 */
void PlayerManager::loadLevel(TiXmlElement* levelBirdElement)
{
	if (bodyBlue1 != NULL)
	{
		PhysicsWorld::world->DestroyBody(bodyBlue1);
		bodyBlue1 = NULL;
	}
	if (bodyBlue2 != NULL)
	{
		PhysicsWorld::world->DestroyBody(bodyBlue2);
		bodyBlue2 = NULL;
	}
	split = false;
	PhysicsWorld::getInstance()->setSleeping(true);

	levelBirdElement->QueryFloatAttribute("originX", &birdOriginX);
	levelBirdElement->QueryFloatAttribute("originY", &birdOriginY);

	currentBirdElement = levelBirdElement->FirstChildElement();

	birdCount = 0;
	while (currentBirdElement != NULL)
	{
		birdCount++;
		currentBirdElement = currentBirdElement->NextSiblingElement();
	}
	CGame::GetInstance()->setBirdCount(birdCount);

	poofAnimationDeltaTime = 0;
	birdPoofing = false;
	poofX = 0;
	poofY = 0;

	currentBirdElement = levelBirdElement->FirstChildElement();
	loadNextBird();
}

/**
 *	@method PlayerManager::loadNextBird()
 *	@desc   instantiates the next bird in the level
 */
void PlayerManager::loadNextBird()
{
	if (bodyBlue1 != NULL)
	{
		PhysicsWorld::world->DestroyBody(bodyBlue1);
		bodyBlue1 = NULL;
	}
	if (bodyBlue2 != NULL)
	{
		PhysicsWorld::world->DestroyBody(bodyBlue2);
		bodyBlue2 = NULL;
	}
	split = false;
	if (currentBirdElement == NULL)
		return;

	currentBirdElement->QueryFloatAttribute("wid", &width);
	height = width;

	body = NULL;

	birdAiming = false;
	birdReleased = false;

	birdSpriteX = birdOriginX;
	birdSpriteY = birdOriginY;

	const char* currentBirdType = currentBirdElement->Attribute("type");

	int i;
	for (i = 0; i < totalPossibleBirds; i++)
	{
		if (strcmp(currentBirdType, allBirdsData[i].objectType) == 0)
		{
			currentBirdData = allBirdsData[i];
			break;
		}
	}
	assert(i < totalPossibleBirds);

	currentBirdElement = currentBirdElement->NextSiblingElement();
	bombBirdAnimationFrame = 2;
	deltaTime = 0;
}

/**
 *	@method PlayerManager::update()
 *	@desc   called every frame, checks the bird status liek aiming, released or not etc.
 */
void PlayerManager::update(unsigned long millis)
{

	if (birdPoofing)
	{
		poofAnimationDeltaTime += millis;
		if (poofAnimationDeltaTime >= 41)
		{
			poofAnimationDeltaTime = 0;
			birdPoofFrame++;
			if (birdPoofFrame == poofData.numSprites)
			{
				birdPoofing = false;
			}
		}
	}

	float distanceX, distanceY, distance, birdAngle;
	if (!birdReleased)
	{
		if (InputManagerC::GetInstance()->isMouseDown())
		{
			float mousex = InputManagerC::GetInstance()->getMouseX();
			float mousey = InputManagerC::GetInstance()->getMouseY();
			if (!birdAiming)
			{
				if (mousex > birdSpriteX && mousex < (birdSpriteX + width))
				{
					if (mousey > birdSpriteY && mousey < (birdSpriteY + height))
					{
						birdAiming = true;
					}
				}
			}
			else
			{
				birdSpriteX = mousex;
				birdSpriteY = mousey - height;

				distanceX = birdSpriteX - birdOriginX;
				distanceY = birdSpriteY - birdOriginY;
				if (distanceX*distanceX + distanceY*distanceY >= 22500)
				{
					birdAngle = atan2(distanceY, distanceX);
					birdSpriteX = birdOriginX + 150 * cos(birdAngle);
					birdSpriteY = birdOriginY + 150 * sin(birdAngle);
				}
			}
		}
		else
		{
			if (birdAiming)
			{
				birdAiming = false;
				birdReleased = true;
				SoundManagerC::GetInstance()->PlayBirdReleaseSound();
				createBirdBody(birdSpriteX, birdSpriteY);
				
				distanceX = birdSpriteX - birdOriginX;
				distanceY = birdSpriteY - birdOriginY;
				distance = sqrt(distanceX * distanceX + distanceY * distanceY);
				birdAngle = atan2(distanceY, distanceX);

				b2Vec2 velocity;
				velocity.Set(-distance * cos(birdAngle) / 6, -distance * sin(birdAngle) / 6);
				body->SetLinearVelocity(velocity);

				birdLifetime = 0;
			}
		}
	}
	else
	{
		birdLifetime += millis;
		
		if (InputManagerC::GetInstance()->isMouseClicked() && birdUserData.collided == false && birdUserData.specialPowerUsed == false)
		{
			if (strcmp(currentBirdData.objectType, "BIRD_YELLOW") == 0)
			{
				b2Vec2 birdVelocity = body->GetLinearVelocity();
				birdVelocity.x = birdVelocity.x * 3;
				birdVelocity.y = birdVelocity.y * 3;

				body->SetLinearVelocity(birdVelocity);
				birdUserData.specialPowerUsed = true;
			}
			else if (strcmp(currentBirdData.objectType, "BIRD_BOMB") == 0)
			{
				printf("explosion\n");
				PhysicsWorld::getInstance()->explode(body->GetWorldCenter(), 200);
				birdUserData.specialPowerUsed = true;

				birdPoofing = true;
				birdPoofFrame = 0;
				poofX = body->GetPosition().x * PhysicsWorld::RATIO;
				poofY = body->GetPosition().y * PhysicsWorld::RATIO;

				birdLifetime = 10001;
			}
			else if (strcmp(currentBirdData.objectType, "BIRD_BLUE") == 0)
			{
				printf("split\n");
				//make others here
				createBirdBodyBlue1(body->GetPosition().x* PhysicsWorld::RATIO, (body->GetPosition().y* PhysicsWorld::RATIO) +1);
				createBirdBodyBlue2(body->GetPosition().x* PhysicsWorld::RATIO, (body->GetPosition().y* PhysicsWorld::RATIO) -1);
				b2Vec2 birdVelocity = body->GetLinearVelocity();
				b2Vec2 birdVelocity1;
				birdVelocity1.y = birdVelocity.x*sin(30 * 3.14159 / 180) + birdVelocity.y*cos(30 * 3.14159 / 180);
				birdVelocity1.x = birdVelocity.x*cos(30 * 3.14159 / 180) - birdVelocity.y*sin(30 * 3.14159 / 180);
				bodyBlue1->SetLinearVelocity(birdVelocity1);
				//bodyBlue1->SetAngularVelocity(body->GetAngle() + (45 * 3.14159 / 180));
				//birdVelocity.y -= 4;
				b2Vec2 birdVelocity2;
				birdVelocity2.y = birdVelocity.x*sin(-15 * 3.14159 / 180) + birdVelocity.y*cos(-15 * 3.14159 / 180);
				birdVelocity2.x = birdVelocity.x*cos(-15 * 3.14159 / 180) - birdVelocity.y*sin(-15 * 3.14159 / 180);
				bodyBlue2->SetLinearVelocity(birdVelocity2);
				//bodyBlue1->SetAngularVelocity(body->GetAngle() + (-45 * 3.14159 / 180));
				birdUserData.specialPowerUsed = true;
				split = true;

				birdPoofing = true;
				birdPoofFrame = 1;
				poofX = body->GetPosition().x * PhysicsWorld::RATIO;
				poofY = body->GetPosition().y * PhysicsWorld::RATIO;
			}
		}

		float bodyPosX = body->GetPosition().x * PhysicsWorld::RATIO;
		if (bodyPosX < 0 || bodyPosX > CGame::mScreenWidth || birdLifetime > 5000)
		{
			PhysicsWorld::world->DestroyBody(body);
			body = NULL;

			birdSpriteX = birdOriginX;
			birdSpriteY = birdOriginY;

			birdReleased = false;
			birdLifetime = 0;

			CGame::GetInstance()->decrementBirdCount();
			loadNextBird();
		}
		//if (strcmp(currentBirdData.objectType, "BIRD_BOMB") == 0)
		//{
		//	deltaTime += millis;
		//	if (bombBirdAnimationFrame >= currentBirdData.numSprites)
		//	{
		//		bombBirdAnimationFrame = 1;
		//	}
		//	else if (deltaTime > 100)
		//	{
		//		bombBirdAnimationFrame++;
		//		deltaTime = 0;
		//	}
		//}
		if (birdLifetime > 5000)
		{
			if (bodyBlue1 != NULL)
			{
				PhysicsWorld::world->DestroyBody(bodyBlue1);
				bodyBlue1 = NULL;
			}
			if (bodyBlue2 != NULL)
			{
				PhysicsWorld::world->DestroyBody(bodyBlue2);
				bodyBlue2 = NULL;
			}
			split = false;
		}
	}

}

/**
 *	@method PlayerManager::render()
 *	@desc   calls the catapult and bird renderer
 */
void PlayerManager::render()
{	
	// back side of catapult
	playerRenderer->setXPositionLeft(rCatapultXLeft);
	playerRenderer->setXPositionRight(rCatapultXRight);
	playerRenderer->setYPositionTop(rCatapultYTop);
	playerRenderer->setYPositionBottom(rCatapultYBottom);
	playerRenderer->render(0);

	if (birdAiming)
	{
		float xDist = rCatapultXLeft + 20 - birdSpriteX;
		float yDist = rCatapultYBottom - 30 - (birdSpriteY + height / 2);
		float aimAngle = atan2(yDist, xDist);

		xDist = xDist * xDist;
		yDist = yDist * yDist;

		playerRenderer->setXPositionLeft(rCatapultXLeft + 20 - (sqrt(xDist + yDist)));
		playerRenderer->setXPositionRight(rCatapultXLeft + 20);
		playerRenderer->setYPositionTop(rCatapultYBottom - 40);
		playerRenderer->setYPositionBottom(rCatapultYBottom - 40 + 20);

		playerRenderer->render(2, aimAngle * 180 / 3.1415, 20, 2 * abs(sqrt(xDist + yDist))); // 2 * sqrt(xDist + yDist));
	}

	//birds left
	TiXmlElement* BirdElement = currentBirdElement;
	float widthBird, heightBird;
	int x = lCatapultXLeft;
	int y = 144;
	birdY += jumpChange;
	if (birdY == 188)
	{
		jumpChange *= -1;
	}
	if (birdY == 144)
	{
		jumpChange *= -1;
		birdJumping++;
		if (birdJumping == birdCount)
		{
			birdJumping = 0;
		}
	}
	int j = 0;
	while (BirdElement != NULL)
	{
		BirdElement->QueryFloatAttribute("wid", &widthBird);
		BirdElement->QueryFloatAttribute("ht", &heightBird);
		x -= widthBird;
		const char* currentBirdType = BirdElement->Attribute("type");
		int i;
		ObjectData BirdData;
		for (i = 0; i < totalPossibleBirds; i++)
		{
			if (strcmp(currentBirdType, allBirdsData[i].objectType) == 0)
			{
				BirdData = allBirdsData[i];
				break;
			}
		}
		int birdSpriteIndex = BirdData.spriteData[0].rendererIndex;
		if (j == 0 && !birdReleased)
		{
			firstBirdX = x;
			firstBirdY = birdY;
		}
		if (birdReleased && j == 0)
		{
			if (firstBirdX != x + 53 && firstBirdY >= 308 - 53)
			{
				firstBirdX++;
			}
			if (firstBirdY != 308)
			{
				firstBirdY++;
			}
			GLfloat xPositionRight = firstBirdX + widthBird;
			GLfloat yPositionBottom = firstBirdY + heightBird;

			playerRenderer->setXPositionLeft(firstBirdX);
			playerRenderer->setXPositionRight(xPositionRight);

			playerRenderer->setYPositionTop(firstBirdY);
			playerRenderer->setYPositionBottom(yPositionBottom);
		}
		else
		{
			playerRenderer->setXPositionLeft(x);
			playerRenderer->setXPositionRight(x + widthBird);
			if (j == birdJumping)
			{
				playerRenderer->setYPositionTop(birdY);
				playerRenderer->setYPositionBottom(birdY + heightBird);
			}
			else
			{
				playerRenderer->setYPositionTop(144);
				playerRenderer->setYPositionBottom(144 + heightBird);
			}
		}
		
		
		playerRenderer->render(birdSpriteIndex);
		
		BirdElement = BirdElement->NextSiblingElement();
		j++;
	}
	

	// bird
	float32 angle = 0;
	int birdSpriteIndex = currentBirdData.spriteData[0].rendererIndex;
	if (birdReleased)
	{
		b2Vec2 position = body->GetPosition();
		angle = body->GetAngle();

		GLfloat xPositionLeft = (position.x * PhysicsWorld::RATIO) - width / 2;
		GLfloat xPositionRight = xPositionLeft + width;

		GLfloat yPositionTop = ((position.y * PhysicsWorld::RATIO));
		GLfloat yPositionBottom = yPositionTop + height;

		playerRenderer->setXPositionLeft(xPositionLeft);
		playerRenderer->setXPositionRight(xPositionRight);

		playerRenderer->setYPositionTop(yPositionTop);
		playerRenderer->setYPositionBottom(yPositionBottom);
		birdSpriteIndex = currentBirdData.spriteData[1].rendererIndex;
	}
	else
	{
		playerRenderer->setXPositionLeft(birdSpriteX);
		playerRenderer->setXPositionRight(birdSpriteX + width);

		playerRenderer->setYPositionTop(birdSpriteY);
		playerRenderer->setYPositionBottom(birdSpriteY + height);
	}

	if (!birdReleased)
	{
		if (CGame::GetInstance()->GetBirdCount() != 0)
			playerRenderer->render(birdSpriteIndex);
	}
	else
	{

		if (strcmp(currentBirdData.objectType, "BIRD_YELLOW") == 0)
		{
			if (birdUserData.specialPowerUsed)
			{
				birdSpriteIndex = currentBirdData.spriteData[2].rendererIndex;
			}
			if (birdUserData.collided)
			{
				birdSpriteIndex = currentBirdData.spriteData[3].rendererIndex;
			}
		}
		else if (strcmp(currentBirdData.objectType, "BIRD_BOMB") == 0)
		{
			if (birdUserData.specialPowerUsed)
			{
				birdSpriteIndex = currentBirdData.spriteData[bombBirdAnimationFrame].rendererIndex;
			}
			if (birdUserData.collided)
			{
				birdSpriteIndex = currentBirdData.spriteData[1].rendererIndex;
			}
		}
		else if (birdUserData.collided)
		{
			birdSpriteIndex = currentBirdData.spriteData[currentBirdData.numSprites - 1].rendererIndex;
		}
		playerRenderer->render(birdSpriteIndex, angle, height, width);
		if (split)
		{
			b2Vec2 position = bodyBlue1->GetPosition();
			angle = bodyBlue1->GetAngle();

			GLfloat xPositionLeft = (position.x * PhysicsWorld::RATIO) - width / 2;
			GLfloat xPositionRight = xPositionLeft + width;

			GLfloat yPositionTop = ((position.y * PhysicsWorld::RATIO));
			GLfloat yPositionBottom = yPositionTop + height;

			playerRenderer->setXPositionLeft(xPositionLeft);
			playerRenderer->setXPositionRight(xPositionRight);

			playerRenderer->setYPositionTop(yPositionTop);
			playerRenderer->setYPositionBottom(yPositionBottom);
			birdSpriteIndex = currentBirdData.spriteData[0].rendererIndex;
			playerRenderer->render(birdSpriteIndex, angle, height, width);

			position = bodyBlue2->GetPosition();
			angle = bodyBlue2->GetAngle();

			xPositionLeft = (position.x * PhysicsWorld::RATIO) - width / 2;
			xPositionRight = xPositionLeft + width;

			yPositionTop = ((position.y * PhysicsWorld::RATIO));
			yPositionBottom = yPositionTop + height;

			playerRenderer->setXPositionLeft(xPositionLeft);
			playerRenderer->setXPositionRight(xPositionRight);

			playerRenderer->setYPositionTop(yPositionTop);
			playerRenderer->setYPositionBottom(yPositionBottom);
			birdSpriteIndex = currentBirdData.spriteData[0].rendererIndex;
			playerRenderer->render(birdSpriteIndex, angle, height, width);
		}
	}

	if (birdPoofing)
	{
		float poofFrameWidth = poofData.spriteData[birdPoofFrame].uRight - poofData.spriteData[birdPoofFrame].uLeft;
		float poofFrameHeight = poofData.spriteData[birdPoofFrame].vBottom - poofData.spriteData[birdPoofFrame].vTop;

		playerRenderer->setXPositionLeft(poofX - poofFrameWidth / 2);
		playerRenderer->setXPositionRight(poofX + poofFrameWidth / 2);

		playerRenderer->setYPositionTop(poofY - poofFrameHeight / 2);
		playerRenderer->setYPositionBottom(poofY + poofFrameHeight / 2);

		playerRenderer->render(poofData.spriteData[birdPoofFrame].rendererIndex);
	}

	// front side of catapult
	playerRenderer->setXPositionLeft(lCatapultXLeft);
	playerRenderer->setXPositionRight(lCatapultXRight);
	playerRenderer->setYPositionTop(lCatapultYTop);
	playerRenderer->setYPositionBottom(lCatapultYBottom);
	playerRenderer->render(1);

	if (birdAiming)
	{
		float xDist = lCatapultXLeft + 10 - (birdSpriteX);
		float yDist = lCatapultYBottom - 30 - (birdSpriteY + height / 2);
		float aimAngle = atan2(yDist, xDist);

		xDist = xDist * xDist;
		yDist = yDist * yDist;

		playerRenderer->setXPositionLeft(lCatapultXLeft + 10 - (sqrt(xDist + yDist)));
		playerRenderer->setXPositionRight(lCatapultXLeft + 10);
		playerRenderer->setYPositionTop(lCatapultYBottom - 40);
		playerRenderer->setYPositionBottom(lCatapultYBottom - 40 + 20);

		playerRenderer->render(2, aimAngle * 180 / 3.1415, 20, 2 * abs(sqrt(xDist + yDist))); // 2 * sqrt(xDist + yDist));
	}

}

/**
 *	@method PlayerManager::~PlayerManager()
 *	@desc   destructor to free up heap allocations
 */
PlayerManager::~PlayerManager()
{
	if (body != NULL)
	{
		PhysicsWorld::world->DestroyBody(body);
	}
	delete playerRenderer;
}

/**
 *	@method PlayerManager::createBirdBody()
 *	@desc   initializes the box2d body of the current bird in the level
 */
void PlayerManager::createBirdBody(float x, float y)
{
	if (body != NULL)
	{
		PhysicsWorld::world->DestroyBody(body);
		body = NULL;
	}

	birdUserData.bodyHealth = currentBirdData.health;
	birdUserData.bodyId = 100;
	birdUserData.collided = false;
	birdUserData.specialPowerUsed = false;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x / PhysicsWorld::RATIO, y / PhysicsWorld::RATIO);
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = &birdUserData;
	bodyDef.bullet = true;

	b2CircleShape bodyShape;
	bodyShape.m_radius = (width / 2.0) / PhysicsWorld::RATIO;

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = currentBirdData.friction;
	bodyFixture.restitution = currentBirdData.restitution;
	bodyFixture.density = currentBirdData.density;

	body = PhysicsWorld::world->CreateBody(&bodyDef);
	body->CreateFixture(&bodyFixture);
}

void PlayerManager::createBirdBodyBlue1(float x, float y)
{
	if (bodyBlue1 != NULL)
	{
		PhysicsWorld::world->DestroyBody(bodyBlue1);
		bodyBlue1 = NULL;
	}

	birdUserData.bodyHealth = currentBirdData.health;
	birdUserData.bodyId = 100;
	birdUserData.collided = false;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x / PhysicsWorld::RATIO, y / PhysicsWorld::RATIO);
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = &birdUserData;
	bodyDef.bullet = true;

	b2CircleShape bodyShape;
	bodyShape.m_radius = (width / 2.0) / PhysicsWorld::RATIO;

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = currentBirdData.friction;
	bodyFixture.restitution = currentBirdData.restitution;
	bodyFixture.density = currentBirdData.density;
	
	bodyBlue1 = PhysicsWorld::world->CreateBody(&bodyDef);
	bodyBlue1->CreateFixture(&bodyFixture);
}

void PlayerManager::createBirdBodyBlue2(float x, float y)
{
	if (bodyBlue2 != NULL)
	{
		PhysicsWorld::world->DestroyBody(bodyBlue2);
		bodyBlue2 = NULL;
	}

	birdUserData.bodyHealth = currentBirdData.health;
	birdUserData.bodyId = 100;
	birdUserData.collided = false;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x / PhysicsWorld::RATIO, y / PhysicsWorld::RATIO);
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = &birdUserData;
	bodyDef.bullet = true;

	b2CircleShape bodyShape;
	bodyShape.m_radius = (width / 2.0) / PhysicsWorld::RATIO;

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = currentBirdData.friction;
	bodyFixture.restitution = currentBirdData.restitution;
	bodyFixture.density = currentBirdData.density;

	bodyBlue2 = PhysicsWorld::world->CreateBody(&bodyDef);
	bodyBlue2->CreateFixture(&bodyFixture);
}