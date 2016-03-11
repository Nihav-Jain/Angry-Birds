#ifndef RENDERER_H
#define RENDERER_H

/**
 *	@class Renderer
 *	@desc  Handles the rendering of a given texture
 */
class Renderer
{
	public:
		void init(const char* texturePath, int numSp, float imgWid, float imgHt);

		void setXPositionLeft(float xPosLeft);
		void setXPositionRight(float xPosRight);
		void setYPositionTop(float yPosTop);
		void setYPositionBottom(float yPosBottom);

		void setUTextureLeft(int index, float xPosLeft);
		void setUTextureRight(int index, float xPosRight);
		void setVTextureTop(int index, float yPosTop);
		void setVTextureBottom(int index, float yPosBottom);
		
		virtual void render(int index=0, float angle = 0 , int height = 0, int width = 0);

		~Renderer();

	private:
		float xPositionLeft;
		float xPositionRight;
		float yPositionTop;
		float yPositionBottom;

		float imageWidth;
		float imageHeight;

		int numSprites;

		float *uTextureLeft;
		float *uTextureRight;
		float *vTextureTop;
		float *vTextureBottom;

		unsigned int mTextureMap;
};

#endif