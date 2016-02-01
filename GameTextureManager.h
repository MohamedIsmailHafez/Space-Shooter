#ifndef GAMETEXTUREMANAGER_H
#define GAMETEXTUREMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

class GameTextureManager
{
public:
	static GameTextureManager& getInstance()
	{
		static GameTextureManager instance;
		return instance;
	}

	GLuint LoadTexture( const char * filename, bool alpha = false);

	void DrawTextured(float centerX, float centerY, float width, float height, GLuint texture);

	void LoadTextures();

	GLuint Player_Ship_Texture, explosionTex, enemyTexI, enemyTexII;

private:
	GameTextureManager() {}
	virtual ~GameTextureManager();
	GameTextureManager(GameTextureManager const&);
	void operator=(GameTextureManager const&);
};
#endif