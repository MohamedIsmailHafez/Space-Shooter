#ifndef ENEMYSHIP_H
#define ENEMYSHIP_H

#include "Ship.h"

class EnemyShip : public Ship
{
public:
	EnemyShip(const Vector2& fPosition, const Vector2& fDirection, float fDirectionAngle, int fLevel);
	virtual ~EnemyShip();

protected:
	virtual void GLRender() override;
	virtual void Update(float fDeltaTime) override;

private:
	int mLevel;
};
#endif