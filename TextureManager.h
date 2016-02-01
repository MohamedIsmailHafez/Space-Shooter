#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

class TextureManager
{
public:
	GLuint LoadTexture( const char * filename, bool alpha = false);

	void DrawTextured(float centerX, float centerY, float width, float height, GLuint texture);
};