#ifndef PLAYER_MANAGER
#define PLAYER_MANAGER

#include "Box2D\Box2D.h"
#include "TinyXML\tinyxml.h"

#include "Renderer.h"
#include "HashMap.h"

/**
*	@class PlayerManager
*	@desc  Singleton handling the initialization, update and rendering of the bird
*/
class PlayerManager
{
	public:
		static PlayerManager* getInstance();
		static PlayerManager* createInstance();
		
		void init(TiXmlElement* birdsElement, TiXmlElement* catapultElement, TiXmlElement* poofElement);
		void update(unsigned long milis);
		void render();
		void loadLevel(TiXmlElement* levelBirdElement);
		~PlayerManager();
	private:
		PlayerManager(){};
		static PlayerManager* sInstance;

		void createBirdBody(float x, float y);
		void createBirdBodyBlue1(float x, float y);
		void createBirdBodyBlue2(float x, float y);
		void loadNextBird();

		ObjectData* allBirdsData;
		int totalPossibleBirds;

		TiXmlElement* currentBirdElement;

		b2Body *body;
		b2Body *bodyBlue1 = NULL;
		b2Body *bodyBlue2 = NULL;

		bool split = false;

		float birdSpriteX;
		float birdSpriteY;
		float width;
		float height;

		float birdOriginX;
		float birdOriginY;
		
		bool birdAiming;
		bool birdReleased;
		bool mouseNowUp;

		float rCatapultXLeft;
		float rCatapultXRight;
		float rCatapultYTop;
		float rCatapultYBottom;

		float lCatapultXLeft;
		float lCatapultXRight;
		float lCatapultYTop;
		float lCatapultYBottom;

		int jumpChange = 1;
		int birdY = 144;
		int birdJumping = 0;
		int birdCount;
		int firstBirdX, firstBirdY;

		int bombBirdAnimationFrame;
		unsigned long deltaTime;

		ObjectData currentBirdData;

		DWORD birdLifetime;
		Renderer *playerRenderer;

		BodyUserData birdUserData;

		bool birdPoofing;
		int birdPoofFrame;
		DWORD poofAnimationDeltaTime;
		ObjectData poofData;
		float poofX;
		float poofY;
};

#endif