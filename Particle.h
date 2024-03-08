#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "SystemConstraints.h"
#include "CircleModel.h"
#include "NeuralNet.h"

class Particle : public CircleModel {
private:
	sf::CircleShape shape;
	NeuralNet* nn;
	bool osc = true;

public:
	Particle(float posX, float posY, float radius, float mass, std::vector<float> &forceSensitivity)
		: CircleModel{ sf::Vector2f(posX, posY), radius, mass, forceSensitivity }
	{

		nn = new NeuralNet(11, new std::vector<int>{ 10, 10, 10 }, 5);
		nn->randInit();

		shape.setPosition(getPos());
		shape.setFillColor(sf::Color::White);
		shape.setRadius(radius * SCALE);
		shape.setOrigin(radius * SCALE, radius * SCALE);
	}

	Particle(float posX, float posY, float radius, float mass, std::vector<float>& forceSensitivity, NeuralNet* parentNN)
		: CircleModel{ sf::Vector2f(posX, posY), radius, mass, forceSensitivity }
	{
		nn = (parentNN->copy());
		nn->mutate(MUTATION_RATE);

		shape.setPosition(getPos());
		shape.setFillColor(sf::Color::White);
		shape.setRadius(radius * SCALE);
		shape.setOrigin(radius * SCALE, radius * SCALE);
	}

	~Particle() {
		delete &shape;
		delete &nn;
	}

	void forward() {
		float tmp[] = {
			1.0f,
			osc,
			getVel().x,
			getVel().y,
			getForce().x,
			getForce().y,
			getForceSensitivity(0),
			getForceSensitivity(1),
			getForceSensitivity(2),
			getForceSensitivity(3),
			getForceSensitivity(4),
		};
		float* output = nn->forward(tmp);

		//std::cout << "New Force Sensitivity: [0] = " << output[0] << std::endl;

		setForceSensitivity(0, output[0]);
		setForceSensitivity(1, output[1]);
		setForceSensitivity(2, output[2]);
		setForceSensitivity(3, output[3]);
		setForceSensitivity(4, output[4]);

		osc = !osc;
	}

	Particle* createChild() {
		return new Particle(std::rand() % WIDTH, std::rand() % HEIGHT, getRadius(), getMass(), *getForceSensitivityMap(), nn);
	}

	void render(sf::RenderWindow& window) {
		shape.setPosition(getPos() * SCALE);
		window.draw(shape);
	}

	

};

#endif