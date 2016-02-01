#include "Bullet.h"

Bullet::Bullet(const Vector2& fPosition, const Vector2& fDirection)
	: GameObject(fPosition, fDirection, 0, GameObjectType::BULLET)
{

}

Bullet::~Bullet()
{

}

void Bullet::GLRender()
{
	glLoadIdentity();

	glColor3f(1,0,0);

	glBegin(GL_POINTS);
	glVertex2f(mPosition.x, mPosition.y);
	glEnd();
}

void Bullet::Update(float fDeltaTime)
{
	mPosition += mDirection * fDeltaTime;
}
