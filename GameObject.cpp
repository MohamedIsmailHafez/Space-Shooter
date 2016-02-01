#include "GameObject.h"

GameObject::GameObject(const Vector2& fPosition, const Vector2& fDirection, float fDirectionAngle, GameObjectType fType)
	: mPosition(fPosition), mDirection(fDirection), mDirectionangle(fDirectionAngle), mType(fType) 
{

}

GameObject::~GameObject()
{

}