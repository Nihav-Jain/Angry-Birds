#ifndef LOSE_SCREEN_H
#define LOSE_SCREEN_H

#include "Renderer.h"

class LoseScreen
{
public:
	void init(char* imageSrc);
	void render(int index = 0);
	void loadText(char* imageSrc);
	~LoseScreen();
private:
	Renderer* LoseScreenRenderer;
};

#endif