#pragma once
#ifndef PHYSICSMODEL_H
#define PHYSICSMODEL_H
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "SystemConstraints.h"

class PhysicsModel {
private:
	sf::Vector2f positon;
	sf::Vector2f velocity;
	sf::Vector2f force;
	float mass;
	std::vector<float> forceSensitivity;
	float minDist = 1.0f;

public:

	PhysicsModel(sf::Vector2f positon, float mass, std::vector<float> forceSensitivity)
		: positon(positon), mass(mass), forceSensitivity(forceSensitivity) {}

	~PhysicsModel() {
		delete &positon;
		delete &velocity;
		delete &force;
		delete[] &forceSensitivity;
	}

	virtual void positonUpdate()
	{
		changeVel(force / mass);
		setForce(0, 0);
		changePos(velocity * STEPPING);

		if(BORDERS) {
			if (positon.x < 0) { positon.x = 0; velocity.x *= -WALL_COLLISON_EFFICIENCY; }
			else if (positon.x > WIDTH) { positon.x = WIDTH; velocity.x *= -WALL_COLLISON_EFFICIENCY; }
			else if (positon.y < 0) { positon.y = 0; velocity.y *= -WALL_COLLISON_EFFICIENCY; }
			else if (positon.y > HEIGHT) { positon.y = HEIGHT; velocity.y *= -WALL_COLLISON_EFFICIENCY; }
		}
	}

	float getMass() const { return mass; }
	sf::Vector2f getPos() const { return positon; }
	sf::Vector2f getVel() const { return velocity; }
	sf::Vector2f getForce() const { return force; }
	float getMinDist() const { return minDist; }
	float getForceSensitivity(int id) { return forceSensitivity[id]; }
	std::vector<float>* getForceSensitivityMap() { return &forceSensitivity; }
	
	void setForceSensitivity(int id, float value) { forceSensitivity[id] = value; }

	void changePos(sf::Vector2f newPos) { positon += newPos; }
	void changePos(float posX, float posY) { positon.x += posX; positon.y += posY; }
	void setPos(sf::Vector2f newPos) { positon = newPos; }
	void setPos(float posX, float posY) { positon.x = posX; positon.y = posY; }

	void changeVel(sf::Vector2f newVel) { velocity += newVel; }
	void changeVel(float velX, float velY) { velocity.x += velX; velocity.y += velY; }
	void setVel(sf::Vector2f newVel) { velocity = newVel; }
	void setVel(float velX, float velY) { velocity.x = velX; velocity.y = velY; }

	void changeForce(sf::Vector2f newForce) { force += newForce; }
	void changeForce(float forceX, float forceY) { force.x += forceX; force.y += forceY; }
	void setForce(sf::Vector2f newForce) { force = newForce; }
	void setForce(float forceX, float forceY) { force.x = forceX; force.y = forceY; }

	void setMinDist(float newMinDist) { minDist = newMinDist; }

	virtual void render(sf::RenderWindow &window) = 0;

};

#endif