#pragma once
#ifndef FORCE_H
#define FORCE_H
#include <vector>
#include <functional>
#include "PhysicsModel.h"

class Force
{
private:
	uint8_t id;
	float forceMultiplier;
	bool isLongRange = true;
	float range = 0;

public:
	Force(uint8_t id, float forceMultiplier)
	{
		this->id = id;
		this->forceMultiplier = forceMultiplier;
		this->isLongRange = true;
	}
	Force(uint8_t id, float forceMultiplier, float range)
	{
		this->id = id;
		this->forceMultiplier = forceMultiplier;
		this->isLongRange = false;
		this->range = range;
	}

	float getForceMultiplier() { return forceMultiplier; }
	bool getIsLongRange() { return isLongRange; }
	float getRange() { return range; }
	uint8_t getId() const { return id; }

	virtual void applyForces(PhysicsModel* m1, PhysicsModel* m2) {
		if (&m1 != &m2)
		{
			sf::Vector2f distXY = m1->getPos() - m2->getPos();
			float dist = std::hypot(distXY.x , distXY.y);

			if (!getIsLongRange())
				if (getRange() < dist)
					return;

			float inverseDist = 1.0f / clip(dist, m1->getMinDist() + m2->getMinDist(), dist);
			distXY *= inverseDist;
			float inverseSquareDist = inverseDist * inverseDist;

			float force = getForceMultiplier() * m1->getForceSensitivity(getId()) * m2->getForceSensitivity(getId()) * m1->getMass() * m2->getMass() * inverseSquareDist;
			sf::Vector2f forceXY = sf::Vector2f(distXY.x * force, distXY.y * force);

			m1->changeForce(forceXY);
			m2->changeForce(-forceXY);

		}
	}

};

#endif
