#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/Graphics.hpp>
#include <map>
#include "Force.h"

class Particle
{
private:
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f acc;
	sf::CircleShape shape;
	float mass;

	std::map<Force, float> *forceSensitivity;

public:
	Particle(float posX, float posY, float mass, std::map<Force, float> forceSensitivity)
	{
		this->pos.x = posX;
		this->pos.y = posY;
		this->mass = mass;
		this->forceSensitivity = *forceSensitivity;

		shape.setPosition(pos);
		shape.setFillColor(sf::Color::White);
		shape.setRadius(3);
	}

	float getMass() { return mass; }
	sf::Vector2f getPos() { return pos; }
	sf::Vector2f getVel() { return vel; }
	sf::Vector2f getAcc() { return acc; }
	float getParticleSensitivity(Force *f) { return forceSensitivity->at(*f); }

	void changeVel(sf::Vector2f newVel) { vel += newVel; }
	void changeVel(float velX, float velY) { vel.x += velX; vel.y += velY; }
	void changeAcc(sf::Vector2f newAcc) { acc += newAcc; }
	void changeAcc(float accX, float accY) { acc.x += accX; acc.y += accY; }

	void render(sf::RenderWindow &window) { window.draw(shape); }

	void positonUpdate()
	{
		vel += acc;
		pos += vel;
		shape.setPosition(pos);
		acc.x = 0;
		acc.y = 0;
	}

};

#endif