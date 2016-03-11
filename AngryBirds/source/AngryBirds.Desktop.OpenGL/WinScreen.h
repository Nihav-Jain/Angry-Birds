#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include "Renderer.h"

class WinScreen
{
public:
	void init(char *imageSrc);
	void render(int index = 0);
	void loadText(char *imageSrc);
	~WinScreen();
private:
	Renderer* WinScreenRenderer;
};

#endif