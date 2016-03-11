#ifndef ANGRY_BIRDS_MOUSE_H
#define ANGRY_BIRDS_MOUSE_H

#include "TinyXML\tinyxml.h"
#include "Renderer.h"

/**
 *	@class AngryBirdsMouse
 *	@desc Singleton class which handles the mouse cursor and cursor states
 */
class AngryBirdsMouse
{
	public:
		static AngryBirdsMouse* getInstance();
		void init(TiXmlElement* mouseElement);
		void update();
		void render();
		~AngryBirdsMouse();

	private:
		AngryBirdsMouse(){};
		static AngryBirdsMouse* sInstance;

		Renderer* mouseRenderer;
		int renderIndex;

		int cursorWidth;
		int cursorHeight;
};

#endif