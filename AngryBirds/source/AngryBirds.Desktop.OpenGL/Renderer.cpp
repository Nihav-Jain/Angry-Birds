#include "stdafx.h"
#include "Renderer.h"

/**
 *	@method Renderer::init()
 *	@desc   loads the texture and stores images related data
 */
void Renderer::init(const char* texturePath, int numSp, float imgWid, float imgHt)
{
	numSprites = numSp;
	mTextureMap = SOIL_load_OGL_texture(texturePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	imageWidth = imgWid;
	imageHeight = imgHt;

	xPositionLeft = 0;
	xPositionRight = xPositionLeft; // +spriteWidth;
	yPositionTop = 0;
	yPositionBottom = yPositionTop; // +spriteHeight;

	uTextureLeft = (float *)malloc(numSprites * sizeof(float));
	uTextureRight = (float *)malloc(numSprites * sizeof(float));
	vTextureTop = (float *)malloc(numSprites * sizeof(float));
	vTextureBottom = (float *)malloc(numSprites * sizeof(float));

}

/**
 *	@method Renderer::render()
 *	@desc   renderes the given sprite index on the set screen position
 */
void Renderer::render(int index, float angle, int height, int width)
{
	assert(index < numSprites);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureMap);	//
	if (angle != 0)
	{
		glPushMatrix();
		glTranslatef(xPositionLeft + (width/2), yPositionBottom-(height/2), 0.0);
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(-xPositionLeft - (width/2), -yPositionBottom + (height/2), 0.0);
	}
	glBegin(GL_QUADS);	

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glTexCoord2f(uTextureLeft[index], vTextureTop[index]);
	glVertex3f(xPositionLeft, yPositionTop, 0.0);

	glTexCoord2f(uTextureRight[index], vTextureTop[index]);
	glVertex3f(xPositionRight, yPositionTop, 0.0);

	glTexCoord2f(uTextureRight[index], vTextureBottom[index]);
	glVertex3f(xPositionRight, yPositionBottom, 0.0);

	glTexCoord2f(uTextureLeft[index], vTextureBottom[index]);
	glVertex3f(xPositionLeft, yPositionBottom, 0.0);

	glEnd();
	if (angle != 0)
		glPopMatrix();
}

/**
 *	@method Renderer::setXPositionLeft()
 *	@desc   setter for screen left X coordinate
 */
void Renderer::setXPositionLeft(GLfloat xPosLeft)
{
	xPositionLeft = xPosLeft;
}


/**
 *	@method Renderer::setXPositionRight()
 *	@desc   setter for screen right X coordinate
 */
void Renderer::setXPositionRight(GLfloat xPosRight)
{
	xPositionRight = xPosRight;
}

/**
 *	@method Renderer::setYPositionTop()
 *	@desc   setter for screen top Y coordinate
 */
void Renderer::setYPositionTop(GLfloat yPosTop)
{
	yPositionTop = yPosTop;
}

/**
 *	@method Renderer::setYPositionBottom()
 *	@desc   setter for screen bottom Y coordinate
 */
void Renderer::setYPositionBottom(GLfloat yPosBottom)
{
	yPositionBottom = yPosBottom;
}

/*********************************************/


/**
 *	@method Renderer::setUTextureLeft()
 *	@desc   setter for texture left U coordinate
 */
void Renderer::setUTextureLeft(int index, float xPosLeft)
{
	assert(index >= 0 && index < numSprites);
	uTextureLeft[index] = xPosLeft / imageWidth;
}

/**
 *	@method Renderer::setUTextureRight()
 *	@desc   setter for texture right U coordinate
 */
void Renderer::setUTextureRight(int index, float xPosRight)
{
	assert(index >= 0 && index < numSprites);
	uTextureRight[index] = xPosRight / imageWidth;
}

/**
 *	@method Renderer::setVTextureTop()
 *	@desc   setter for texture top V coordinate
 */
void Renderer::setVTextureTop(int index, float yPosTop)
{
	assert(index >= 0 && index < numSprites);
	vTextureBottom[index] = (imageHeight - yPosTop) / imageHeight;
}

/**
 *	@method Renderer::setVTextureBottom()
 *	@desc   setter for texture bottom V coordinate
 */
void Renderer::setVTextureBottom(int index, float yPosBottom)
{
	assert(index >= 0 && index < numSprites);
	vTextureTop[index] = (imageHeight - yPosBottom) / imageHeight;
}

/**
 *	@method Renderer::~Renderer()
 *	@desc   destructor to free up heap allocations
 */
Renderer::~Renderer()
{
	free(uTextureLeft);
	free(uTextureRight);
	free(vTextureTop);
	free(vTextureBottom);

	if (mTextureMap != 0)
	{
		glDeleteTextures(1, &mTextureMap);
	}
}