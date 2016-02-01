#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"

class PlayerShip : public Ship
{
public:
	PlayerShip();
	virtual ~PlayerShip();
	void SetDirectionAngle(float fDirectionAngle);

protected:
	virtual void GLRender() override;
	virtual void Update(float fDeltaTime) override;
};
#endif
