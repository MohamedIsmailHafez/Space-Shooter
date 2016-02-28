#include "PlayerShip.h"
#include "GameConfig.h"
#include "GameManager.h"
#include "Bullet.h"

PlayerShip::PlayerShip()
	: Ship(Vector2(GameConfig::getInstance().ScreenWidth/2, GameConfig::getInstance().ScreenHeight/2), Vector2(), 0.f, GameObjectType::PLAYER_SHIP)
{
	
}

PlayerShip::~PlayerShip()
{

}

void PlayerShip::GLRender()
{
	glLoadIdentity();

	glTranslatef(mPosition.x, mPosition.y, 0);

	glRotatef(mDirectionangle, 0, 0, 1);

	if(mIsDying)
	{
		GameTextureManager::getInstance().DrawTextured(0, 0, 256, 256, GameTextureManager::getInstance().explosionTex);
	}
	else
	{
#if DEBUG
		glColor3f(0,1,0);
		RenderDebugShip(mPosition.x, mPosition.y);
#else	
		GameTextureManager::getInstance().DrawTextured(0, 0, 64, 64, GameTextureManager::getInstance().Player_Ship_Texture);
#endif
	}
}

void PlayerShip::Update(float fDeltaTime)
{
	if(mIsDying)
		mTimer -= 1.0f;
}

void PlayerShip::SetDirectionAngle(float fDirectionAngle)
{
	if( abs(fDirectionAngle - mDirectionangle) < 0.001f)
		return;

	mDirectionangle = fDirectionAngle;
}

void PlayerShip::Fire(Vector2 fDirection)
{
	Bullet* bullet = new Bullet(Vector2(GameConfig::getInstance().ScreenWidth/2, GameConfig::getInstance().ScreenHeight/2), fDirection);
	GameManager::getInstance().Add(bullet);
}
