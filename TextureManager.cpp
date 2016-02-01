#include "TextureManager.h"

GLuint TextureManager::LoadTexture( const char * filename, bool alpha)
{
	  GLuint texture;

	  unsigned char * data;

	  FILE * file;

	  file = fopen( filename, "rb" );

	  if ( file == NULL ) return 0;
	  
	  unsigned char info[54];
	  fread(info, sizeof(unsigned char), 54, file); // read the 54-byte header

      // extract image height and width from header
      int width = *(int*)&info[18];
      int height = *(int*)&info[22];

	  int channels;

	  if(alpha)
		  channels = 4;
	  else
		  channels = 3;

	  data = (unsigned char *)malloc( width * height * channels );
	  //int size = fseek(file,);
	  fread( data, width * height * channels, 1, file );
	  fclose( file );
	  
	 for(int i = 0; i < width * height ; ++i)
	 {
	   int index = i*channels;
	   unsigned char B,R;
	   B = data[index];
	   R = data[index+2];
	   data[index] = R;
	   data[index+2] = B;
	 }
	 
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	if(alpha)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height,GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data);

	free( data );

	return texture;
}

void TextureManager::DrawTextured(float centerX, float centerY, float width, float height, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f( centerX - width/2, centerY- height/2);
		glTexCoord2f(1, 0); glVertex2f( centerX + width/2, centerY- height/2);
		glTexCoord2f(1, 1); glVertex2f( centerX + width/2, centerY+ height/2);
		glTexCoord2f(0, 1); glVertex2f( centerX - width/2, centerY+ height/2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}