#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(const Vector2& fPosition, const Vector2& fDirection);
	virtual ~Bullet();

protected:
	virtual void GLRender() override;
	virtual void Update(float fDeltaTime) override;

};
#endif