#pragma once
#ifndef NEURALNET_H
#define NEURALNET_H
#include "Layer.h"

class NeuralNet {
private:
	const int inputNum;
	const int outputNum;
	const std::vector<int>* hiddenNum;
	std::vector<Layer*> layers;

public:
	NeuralNet(const int inputNum, const std::vector<int>* hiddenNum, const int outputNum) :
		inputNum(inputNum), outputNum(outputNum), hiddenNum(hiddenNum)
	{
		layers.push_back(new Layer(inputNum, hiddenNum->at(0), ActivationFunctions::Tanh));

		for (int i = 1; i < hiddenNum->size()-1; i++) {
			layers.push_back(new Layer(hiddenNum->at(i-1), hiddenNum->at(i), ActivationFunctions::Tanh));
		}

		layers.push_back(new Layer(hiddenNum->at(hiddenNum->size() - 1), outputNum, ActivationFunctions::Tanh));
	}

	NeuralNet(const int inputNum, const int outputNum, std::vector<Layer*> layers) :
		inputNum(inputNum), outputNum(outputNum), layers(layers)
	{

	}

	float* forward(float *inputs) {
		//float* output = layers[0]->forward(inputs);
		float* tmpOut;
		for (int i = 0; i < layers.size(); i++) {
			tmpOut = layers[i]->forward(inputs);
			for (int j = 0; j < outputNum; j++) {
				inputs[j] = tmpOut[j];
			}
			delete[] tmpOut;
		}

		return inputs;
	}

	NeuralNet* copy() {
		std::vector<Layer*> newLayers;

		for (int i = 0; i < layers.size(); i++) {
			newLayers.push_back(layers[i]->copy());
		}

		return new NeuralNet(inputNum, outputNum, newLayers);
	}

	void mutate(float rate) {
		for (int i = 0; i < layers.size(); i++) {
			layers[i]->mutate(rate);
		}
	}

	void randInit() {
		for (Layer* l : layers) {
			l->initRandom();
		}
	}

};

#endif
