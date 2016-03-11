#include "stdafx.h"
#include "game.h"
#include "BlockManager.h"
#include "PigManager.h"

// Declarations
const char8_t CGame::mGameTitle[] = "Angry Birds";
CGame* CGame::sInstance = NULL;
GameState CGame::mGameState = START_MENU;
bool CGame::destroyed = false;

/**
 *	@method CGame::init()
 *	@desc   inititalizes the Game managers like input manager, sound manager etc.
 */
void CGame::init()
{
	doc = new TiXmlDocument("content/angrybirds.xml");

	levelNumber = 0;

	bool loadOkay = doc->LoadFile();
	assert(loadOkay);

	TiXmlHandle docHandle(doc);

	CGame::destroyed = false;
	CGame::mGameState = START_MENU;
	birdCount = 0;

	InputManagerC::CreateInstance()->init();
	
	PhysicsWorld::getInstance()->init(docHandle.FirstChildElement().ChildElement("physics", 0).Element());

	Background::createInstance()->init(docHandle.FirstChildElement().ChildElement("backgrounds", 0).ChildElement("mainmenu", 0).Element()->Attribute("src"));

	strcpy_s(levelBackgroundSrc, docHandle.FirstChildElement().ChildElement("backgrounds", 0).ChildElement("level", 0).Element()->Attribute("src"));

	score = new Score();
	score->init(docHandle.FirstChildElement().ChildElement("score", 0).Element(), birdCount);

	winScreen = new WinScreen();
	winScreen->init("content/Textures/WinScreen.png");

	loseScreen = new LoseScreen();
	loseScreen->init("content/Textures/LoseScreen.png");

	playButton = new Button();
	playButton->init(docHandle.FirstChildElement().ChildElement("playbutton", 0).Element());

	nextButton = new Button();
	nextButton->init(docHandle.FirstChildElement().ChildElement("nextbutton", 0).Element());

	previousButton = new Button();
	previousButton->init(docHandle.FirstChildElement().ChildElement("previousbutton", 0).Element());

	resetButton = new Button();
	resetButton->init(docHandle.FirstChildElement().ChildElement("resetbutton", 0).Element());

	AngryBirdsMouse::getInstance()->init(docHandle.FirstChildElement().ChildElement("mouse", 0).Element());
	SoundManagerC::CreateInstance()->init();

}

/**
 *	@method CGame::UpdateFrame()
 *	@desc   called every frame, calls the update methods of all other game managers
 */
void CGame::UpdateFrame(DWORD milliseconds)
{
	keyProcess();
	InputManagerC::GetInstance()->update(milliseconds);
	AngryBirdsMouse::getInstance()->update();

	if (CGame::mGameState == LEVEL)
	{
		PhysicsWorld::getInstance()->update(milliseconds);
		level->update(milliseconds);

		if (GetBirdCount() != 0)
			PlayerManager::getInstance()->update(milliseconds);
		BlockManager::getInstance()->update();
		PigManager::getInstance()->update(milliseconds);

		nextButton->update();
		previousButton->update();
		resetButton->update();
		if (nextButton->isButtonPressed())
		{
			levelNumber++;
			loadNextLevel();
		}
		if (previousButton->isButtonPressed())
		{
			levelNumber--;
			if (levelNumber < 0)
			{
				levelNumber = 0;
			}
			loadNextLevel();
		}
		if (resetButton->isButtonPressed())
		{
			loadNextLevel();
		}
	}


	if (CGame::mGameState == START_MENU)
	{
		playButton->update();
		if (playButton->isButtonPressed())
		{
			CGame::mGameState = LEVEL;

			Background::getInstance()->loadBackground(levelBackgroundSrc);

			level = new Level();
			level->init();

			TiXmlHandle docHandle(doc);
			PlayerManager::createInstance()->init(docHandle.FirstChildElement().ChildElement("birds", 0).Element(), docHandle.FirstChildElement().ChildElement("catapult", 0).Element(), docHandle.FirstChildElement().ChildElement("birdpoof", 0).Element());
			BlockManager::createInstance()->init(docHandle.FirstChildElement().ChildElement("blocks", 0).Element());
			PigManager::createInstance()->init(docHandle.FirstChildElement().ChildElement("pigs", 0).Element(), docHandle.FirstChildElement().ChildElement("poof", 0).Element());

			PlayerManager::getInstance()->loadLevel(docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("birds", 0).Element());
			BlockManager::getInstance()->loadLevel(docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("blocks", 0).Element());
			PigManager::getInstance()->loadLevel(docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("pigs", 0).Element());
		}

	}

}

/**
 *	@method CGame::loadNextLevel()
 *	@desc   calls the loadLevel functions of the player, block and pig manager and passses the new level xml to them
 */
void CGame::loadNextLevel()
{
	TiXmlHandle docHandle(doc);
	SoundManagerC::GetInstance()->PauseBG(false);
	if (docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("birds", 0).Element() != NULL)
	{
		PlayerManager::getInstance()->loadLevel(docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("birds", 0).Element());
		BlockManager::getInstance()->loadLevel(docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("blocks", 0).Element());
		PigManager::getInstance()->loadLevel(docHandle.FirstChildElement().ChildElement("level", levelNumber).ChildElement("pigs", 0).Element());
		gameOver = false;
	}
	else
	{
		levelNumber--;
	}
}

/**
 *	@method CGame::DrawScene()
 *	@desc   calls the renderers of all game managers and game entities
 */
void CGame::DrawScene(void)											
{
	startOpenGLDrawing();

	Background::getInstance()->render();

	if (CGame::mGameState == LEVEL)
	{
		level->render();
		PlayerManager::getInstance()->render();
		BlockManager::getInstance()->render();
		PigManager::getInstance()->render();
		
		//score->render();

		if (pigCount == 0)
		{
			winScreen->render();
			if (!gameOver)
			{
				SoundManagerC::GetInstance()->PauseBG();
				SoundManagerC::GetInstance()->PlayLevelComplete();
				gameOver = true;
			}
		}

		else if (birdCount == 0)
		{
			loseScreen->render();
			if (!gameOver)
			{
				SoundManagerC::GetInstance()->PauseBG();
				SoundManagerC::GetInstance()->PlayLevelLost();
				gameOver = true;
			}
		}

		nextButton->render();
		previousButton->render();
		resetButton->render();
	}

	if (CGame::mGameState == START_MENU)
	{
		playButton->render();
	}

	AngryBirdsMouse::getInstance()->render();
}

int CGame::GetBirdCount()
{
	return birdCount;
}


/**
 *	@method CGame::DecrementPigCount()
 *	@desc   called when a pig dies
 */
void CGame::decrementPigCount()
{
	pigCount--;
	if (pigCount <= 0)
	{
		pigCount = 0;
	}
}

/**
 *	@method CGame::decrementBirdCount()
 *	@desc   called when a bird flis out of screen or completes its lifetime
 */
void CGame::decrementBirdCount()
{
	if (!gameOver)
		birdCount--;
	if (birdCount <= 0)
	{
		birdCount = 0;
	}
	score->SetScore(birdCount);
}

/**
 *	@method CGame::setBirdCount()
 *	@desc   called by PlayerManger to set the starting number of birds for the current level
 */
void CGame::setBirdCount(int count)
{
	birdCount = count;
	score->SetScore(birdCount);
}

/**
 *	@method CGame::setPigCount()
 *	@desc   called by the Pig maanger to set the starting number of pigs for the current level
 */
void CGame::setPigCount(int count)
{
	PigStartingCount = count;
	pigCount = PigStartingCount;
}

/**
 *	@method CGame::CreateInstance()
 *	@desc   instantiates and returns the singleton instance for CGame
 */
CGame *CGame::CreateInstance()
{
	sInstance = new CGame();
	return sInstance;
}

/**
 *	@method CGame::shutdown()
 *	@desc   skeleton for destorying heap allocations (currently unused)
 */
void CGame::shutdown()
{

}

/**
 *	@method CGame::DestroyGame()
 *	@desc   called by the CGame destuctor to free up the heap allocated memory
 */
void CGame::DestroyGame(void)
{
	SoundManagerC::GetInstance()->Close();
	delete InputManagerC::GetInstance();
	delete SoundManagerC::GetInstance();
	delete AngryBirdsMouse::getInstance();
	delete Background::getInstance();

	delete playButton;
	delete nextButton;
	delete resetButton;

	if (CGame::mGameState == LEVEL)
	{
		delete level;
		delete PlayerManager::getInstance();
		delete BlockManager::getInstance();
		delete PigManager::getInstance();
	}

	delete PhysicsWorld::getInstance();
	CGame::destroyed = true;
}

/**
 *	@method CGame::~CGame()
 *	@desc   destuctor to free up heap allocated memory
 */
CGame::~CGame()
{
	if (!CGame::destroyed)
	{
		CGame::GetInstance()->shutdown();
		CGame::GetInstance()->DestroyGame();
		CGame::destroyed = true;
	}
}