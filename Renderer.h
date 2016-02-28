#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	static Renderer& getInstance()
	{
		static Renderer instance;
		return instance;
	}

	void InitializeRenderer(int argc, char** argv);
	static int mElapsedTime; 

private:
	static int mLastFrameTime;
	static int mCurrentTime;
	static int mPreviousTime;
	static int mPreviousFPSTime;

	static int mFrameNumber;

	static char mFps[50];
	static char mHintText[200];

	static void Reshape(int fWidth, int fHeight);

	static void Display(void);

	void Keyboard(unsigned char key, int x, int y);

	void InitOpenGL(void);

	static void RenderGameObjects();

	static void CalculateFPS();

	static void RenderText(const char* string, int x, int y, int scale);

	static void DisplayFPS();

	static void DisplayScore();

	static void DisplayHints();

	static void DisplayGameOverScreen();

	void InitializeHintText();
};
#endif