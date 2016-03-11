#ifndef SCORE_H
#define SCORE_H

#include "TinyXML\tinyxml.h"
#include "Renderer.h"

class Score
{
public:
	void init(TiXmlElement* scoreElement, int lives);
	void render();
	void SetScore(int lives);
	~Score();
private:
	Renderer* ScoreRenderer;
	Renderer* TextRenderer;
	int score;
};

#endif