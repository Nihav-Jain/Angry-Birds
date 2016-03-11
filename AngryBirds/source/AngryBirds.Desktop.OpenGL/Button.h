#ifndef BUTTON_H
#define BUTTON_H

#include "TinyXML\tinyxml.h"
#include "Renderer.h"

/**
 *	@class Button
 *	@desc Renders a UI button and provides info whether it is pressed or not
 */
class Button
{
	public:
		void init(TiXmlElement* buttonElement);
		void update();
		void render();
		bool isButtonPressed();
		~Button();
	private:
		float x;
		float y;
		float width;
		float height;
		Renderer* buttonRenderer;
		bool mButtonPressed;
};

#endif