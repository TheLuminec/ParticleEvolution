#pragma once
#ifndef CIRCLEMODEL_H
#define CIRCLEMODEL_H
#include "PhysicsModel.h"

class CircleModel : public PhysicsModel {
private:
	float radius;
	bool collisonEnabled = true;

public:
	CircleModel(sf::Vector2f positon, float radius, float mass, std::vector<float> forceSensitivity)
		: PhysicsModel(positon, mass, forceSensitivity), radius(radius) 
	{
		setMinDist(radius);
	}

	float getRadius() const { return radius; }
	bool getCollisonEnabled() const { return collisonEnabled; }

	void setRadius(float newRadius) { radius = newRadius; }
	void setCollisonEnabled(bool enabled) { collisonEnabled = enabled; }

	template<typename T> 
	std::enable_if_t<std::is_base_of<CircleModel, T>::value>
		checkCollison(std::vector<T*>& models)
	{
		if (collisonEnabled)
		{
			for (CircleModel* p : models)
			{
				if (this != p && p->getCollisonEnabled())
				{
					sf::Vector2f distXY = getPos() - p->getPos();
					float dist = std::hypot(distXY.x, distXY.y);
					float minDist = getRadius() + p->getRadius();

					if (dist < minDist) {
						sf::Vector2f normal = distXY / dist;

						// Apply repulsion force to prevent sticking
						sf::Vector2f repulsion = normal * (minDist - dist) * 0.2f;
						float inverseTotalMass = 1 / (getMass() + p->getMass());

						changePos(getMass() * repulsion * inverseTotalMass);
						p->changePos(-p->getMass() * repulsion * inverseTotalMass);

						sf::Vector2f newVel1 = (((2 * getMass()) * inverseTotalMass) * getVel()) - (((getMass() - p->getMass()) * inverseTotalMass) * p->getVel()) * COLLISON_EFFICIENCY;
						sf::Vector2f newVel2 = (((getMass() - p->getMass()) * inverseTotalMass) * getVel()) - (((2 * p->getMass()) * inverseTotalMass) * p->getVel()) * COLLISON_EFFICIENCY;

						setVel(-newVel2);
						p->setVel(-newVel1);

					}
				}
			}
		}
	}
};

#endif