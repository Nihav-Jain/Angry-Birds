#ifndef PIG_MANAGER_H
#define PIG_MANAGER_H

#include "TinyXML\tinyxml.h"
#include "EnemyPig.h"
#include "HashMap.h"

/**
 *	@class PigManager
 *	@desc  Singleton to handle initialization, update and rendering of pigs
 */
class PigManager
{
	public:
		static PigManager* createInstance();
		static PigManager* getInstance();

		void init(TiXmlElement* pigsElement, TiXmlElement* poofElement);
		void loadLevel(TiXmlElement* levelPigElement);
		void update(DWORD milliseconds);
		void render();
		void shutdown();
		//void startPoofing(float px, float py);

		~PigManager();

	private:
		void poof();

		static PigManager* sInstance;
		EnemyPig** pigList;
		int numPigs;

		Renderer* pigRenderer;
		ObjectData* allPigsData;
		int totalPossiblePigs;

		ObjectData poofData;
		DWORD poofAnimationDeltaTime;
		
		DWORD deltaTime;

};

#endif