#pragma once
#ifndef ACTIVATIONFUNCTION_H
#define ACTIVATIONFUNCTION_H
#include <cmath>

enum ActivationFunctions {
	Sigmoid,
	ReLU,
	LeakyReLU,
	Tanh,
};

static class ActivationFunction {
private:
	static float sigmoid(float input) {

		return input / (1 + abs(input));;
	}

	static float tanh(float input) {

		return std::tanh(input);
	}

	static float reLu(float input) {//need to implement

		return NULL;
	}

	static float leakyReLU(float input) {//need to implement

		return NULL;
	}


public:
	static float forward(ActivationFunctions activationFunction, float input) {
		switch (activationFunction)
		{
			case ActivationFunctions::Sigmoid:
				return sigmoid(input);
			case ActivationFunctions::Tanh:
				return tanh(input);
			case ActivationFunctions::ReLU:
				return reLu(input);
			case ActivationFunctions::LeakyReLU:
				return leakyReLU(input);
			default:
				return NULL;
		}
	}
};

#endif
