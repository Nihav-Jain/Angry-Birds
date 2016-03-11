#include "stdafx.h"
#include "Button.h"


/**
 *	@method Button::init()
 *	@desc   inititalizes the button renderer and screen space coordinates
 */
void Button::init(TiXmlElement *buttonElement)
{
	buttonElement->QueryFloatAttribute("screenX", &x);
	buttonElement->QueryFloatAttribute("screenY", &y);

	float imgWid, imgHt;

	buttonElement->QueryFloatAttribute("imgWidth", &imgWid);
	buttonElement->QueryFloatAttribute("imgHeight", &imgHt);

	buttonRenderer = new Renderer();
	buttonRenderer->init(buttonElement->Attribute("src"), 1, imgWid, imgHt);

	float screenX = x, screenY = y;

	TiXmlElement *buttonSpriteElement = buttonElement->FirstChildElement()->FirstChildElement();
	buttonSpriteElement->QueryFloatAttribute("spriteWidth", &width);
	buttonSpriteElement->QueryFloatAttribute("spriteHeight", &height);

	float spritePosX, spritePosY;
	buttonSpriteElement->QueryFloatAttribute("uPosLeft", &spritePosX);
	buttonSpriteElement->QueryFloatAttribute("vPosTop", &spritePosY);


	buttonRenderer->setUTextureLeft(0, spritePosX);
	buttonRenderer->setUTextureRight(0, spritePosX + width);
	buttonRenderer->setVTextureTop(0, spritePosY);
	buttonRenderer->setVTextureBottom(0, spritePosY + height);

	buttonRenderer->setXPositionLeft(screenX);
	buttonRenderer->setXPositionRight(screenX + width);
	buttonRenderer->setYPositionTop(screenY);
	buttonRenderer->setYPositionBottom(screenY + height);

	mButtonPressed = false;
}


/**
 *	@method Button::update()
 *	@desc   called every frame, checks if button was pressed or not
 */
void Button::update()
{
	mButtonPressed = false;
	if (InputManagerC::GetInstance()->isMouseClicked())
	{
		float mousex = InputManagerC::GetInstance()->getMouseX();
		float mousey = InputManagerC::GetInstance()->getMouseY();

		if (mousex > x && mousex < (x + width))
		{
			if (mousey > y && mousey < (y + height))
			{
				mButtonPressed = true;
			}
		}
	}
}

/**
 *	@method Button::render()
 *	@desc   renders the button sprite
 */
void Button::render()
{
	buttonRenderer->render();
}

/**
 *	@method Button::isButtonPressed()
 *	@desc   getter for button pressed state
 */
bool Button::isButtonPressed()
{
	return mButtonPressed;
}

/**
 *	@method Button::~Button()
 *	@desc   destructor to free up heap allocated memory
 */
Button::~Button()
{
	delete buttonRenderer;
}