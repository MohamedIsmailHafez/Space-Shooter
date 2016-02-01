#include "TextureManager.h"
#include <glut.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <string>
#include <sstream>

int WIDTH  = 800;
int HEIGHT = 600;

TextureManager texManager;
GLuint texture;

//=======================================================================
// Render Text
//=======================================================================
void renderText(char*string, int x, int y, int scale) 
{ 
	 char *c; 
	 glLoadIdentity();
	 glTranslatef(x, y, 0); 
	 glScalef((float)scale/10, (float)scale/10, 1); 
	
	 for (c=string; *c != '\0'; c++) 
	 { 
		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c); 
	 }  
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void Init(void)
{
    glClearColor(0.1,0.5,0.5,0.0); 
 
    glMatrixMode(GL_PROJECTION); 
     
    glLoadIdentity(); 
 
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);  
 
    glMatrixMode(GL_MODELVIEW); 
     
    glLoadIdentity(); 
}

//=======================================================================
// Display Function
//=======================================================================
void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); 

	texManager.DrawTextured(WIDTH/2, HEIGHT/2, 512, 512, texture);

    glFlush();
}

//=======================================================================
// Timer Function
//=======================================================================
void timer(int)
{
    /* update animation */
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, timer, 0);
}

//=======================================================================
// Load Textures Function
//=======================================================================
void LoadTextures()
{
	texture	= texManager.LoadTexture("SpaceShooter.bmp");
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(WIDTH,HEIGHT);  
	glutInitWindowPosition(100, 150); 
    glutCreateWindow("Space Shooter Template"); 
    glutDisplayFunc(Display); 
    Init(); 
	LoadTextures();	//Load Texture files
	glutTimerFunc(1000.0/60.0, timer, 0);	//Assign a function to be invoked in .. milliseconds
    glutMainLoop(); 
}