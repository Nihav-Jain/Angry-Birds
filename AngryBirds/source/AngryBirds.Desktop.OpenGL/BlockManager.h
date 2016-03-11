#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include "TinyXML\tinyxml.h"

#include "HashMap.h"
#include "WoodBlock.h"
#include "Renderer.h"

/**
 *	@class BlockManager
 *	@desc Singleton class which manages the blocks initialization, update and rendering
 */
class BlockManager
{
	public:
		static BlockManager* createInstance();
		static BlockManager* getInstance();

		void init(TiXmlElement* blocksElement);
		void loadLevel(TiXmlElement* levelBlockElement);
		void update();
		void render();
		void shutdown();

		~BlockManager();
	private:
		static BlockManager* sInstance;
		WoodBlock** blockList;
		int numBlocks;

		Renderer* blockRenderer;
		ObjectData* allBlocksData;
		int totalPossibleBlocks;
};

#endif