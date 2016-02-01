#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include "GameObject.h"

enum class GameState {GAMEPLAY, GAMEOVER};

class GameManager
{
public: 
	static GameManager& getInstance()
	{
		static GameManager instance;
		return instance;
	}

	static std::vector<GameObject *> GameObjects;

	void Add(GameObject* fObject);
	void Initialize(GameObject* fPlayerShip);
	int GetScore() { return mScore; }
	GameState GetGameState() { return mGameState; }

private:
	GameManager();
	virtual ~GameManager();
	GameManager(GameManager const&);
	void operator=(GameManager const&);

	void InitializeCallBacks();

	static void MouseMotion(int x, int y);
	static void Mouse(int button, int state, int x, int y);
	static void Keyboard(unsigned char key, int x, int y);
	static void SpawnEnemies(int);
	static void UpdateGameObjects(int);
	static void UpdateScore();

	static bool OutofBounds(Vector2 fPosition);

	static Vector2 GetRandomPosOffScreen();
	static float Distance(Vector2 fPosition1, Vector2 fPosition2);
	static GameObject* mPlayerShip;
	static Vector2 mNormalizedDir;
	static int mScore;
	static GameState mGameState;
};
#endif