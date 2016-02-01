#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2.h"
#include "GameTextureManager.h"
#include <glut.h>

enum class GameObjectType {PLAYER_SHIP, ENEMY_SHIP, BULLET};

class GameObject
{
public:
	GameObject(const Vector2& fPosition, const Vector2& fDirection, float fDirectionAngle, GameObjectType fType);
	virtual ~GameObject();

	virtual void GLRender() = 0;
	virtual void Update(float fDeltaTime) = 0;

	Vector2 GetPosition() const { return mPosition; }
	GameObjectType GetType() const { return mType; }

protected:
	Vector2 mPosition;
	Vector2 mDirection;
	float	mDirectionangle;
	GameObjectType mType;
};

#endif
