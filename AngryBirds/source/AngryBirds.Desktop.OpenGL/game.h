#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameConstants.h"
#include "PhysicsWorld.h"
#include "Level.h"
#include "PlayerManager.h"
#include "Background.h"
#include "Button.h"
#include "AngryBirdsMouse.h"
#include "WoodBlock.h"
#include "EnemyPig.h"
#include "Score.h"
#include "WinScreen.h"
#include "LoseScreen.h"

typedef enum GameState
{
	INVALID,
	START_MENU,
	LEVEL_SELECT,
	LEVEL,
	PAUSED,
	MAX_STATE
} GameState;


/**
*	@class CGame
*	@desc Game Manager Singleton
*/
class CGame
{
public:
	const char* GetGameTitle(){return mGameTitle;}
	static CGame* CreateInstance();
	static CGame* GetInstance() {return sInstance;};
	~CGame();
	void DrawScene();
	void UpdateFrame(DWORD milliseconds);
	void DestroyGame();
	void init();
	void shutdown();
	void decrementBirdCount();
	void decrementPigCount();
	void setBirdCount(int count);
	void setPigCount(int count);
	void DrawScore();
	void loadNextLevel();

	int GetBirdCount();

	static const uint32_t mScreenWidth = SCREEN_WIDTH; //1024;
	static const uint32_t mScreenHeight = SCREEN_HEIGNT; //768;
	static const uint32_t mBitsPerPixel = 32;
	static GameState mGameState;
	static bool destroyed;

private:
	static const char mGameTitle[25];
	static CGame* sInstance;
	CGame(){};

	TiXmlDocument* doc;

	Level* level;

	Button* playButton;
	Button* nextButton;
	Button* previousButton;
	Button* resetButton;

	Score* score;
	WinScreen* winScreen;
	LoseScreen* loseScreen;

	int32 birdCount;
	int32 PigStartingCount;
	int32 pigCount;

	bool gameOver = false;
	char levelBackgroundSrc[50];
	int levelNumber;
};

#endif