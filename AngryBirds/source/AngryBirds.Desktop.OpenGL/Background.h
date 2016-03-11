#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Renderer.h"

/**
 *	@class Background
 *	@desc  Singleton, Handles the background image loading and rendering 
 */
class Background
{
	public:
		static Background* createInstance();
		static Background* getInstance();
		void init(const char* imageSrc);
		void render();
		void loadBackground(const char *imageSrc);
		~Background();
	private:
		static Background* sInstance;
		Renderer* backgroundRenderer;
};

#endif