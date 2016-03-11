#ifndef POOF_H
#define POOF_H

#include "TinyXML\tinyxml.h"
#include "HashMap.h"
#include "Renderer.h"

class Poof
{
	public:
		static Poof* createInstance();
		static Poof* getInstance();

		void init(TiXmlElement* poofElement);
		void update(unsigned long miliseconds);
		void render(int renderIndex);



	private:
		static Poof* sInstance;
		Poof(){};

		Renderer* poofRenderer;

		ObjectData poofData;

		int poofAnimationFrame;
		bool poofing;
		float poofX;
		float poofY;
		unsigned long poofAnimationDeltaTime;


};

#endif