#ifndef GAMECONFIG_H
#define GAMECONFIG_H

class GameConfig
{
public:
	static GameConfig& getInstance()
	{
		static GameConfig instance;
		return instance;
	}

	float ScreenWidth;
	float ScreenHeight;

private:
	GameConfig() {};
	GameConfig(GameConfig const&);
	void operator=(GameConfig const&);
};
#endif