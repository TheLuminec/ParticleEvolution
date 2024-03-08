#pragma once
#ifndef LAYER_H
#define LAYER_H

#include "SystemConstraints.h"
#include "ActivationFunction.h"


class Layer {
private:
	struct Weight {
		float weight = 0;

		float weigh(float input) {
			return input * weight;
		}

	};

	struct Node {
		float sum = 0;
		float bias = 0;

		void add(float input) {
			sum += input;
		}

		float getSum() { 
			float tmpSum = sum;
			sum = 0;

			return tmpSum + bias; 
		}
	};

	const int inputNum;
	const int hiddenNum;

	Weight *weights;
	Node *nodes;
	ActivationFunctions activationFunction;

public:
	Layer();
	Layer(const int inputNum, const int hiddenNum, ActivationFunctions activationFunction) : 
		inputNum(inputNum), hiddenNum(hiddenNum), activationFunction(activationFunction)
	{
		weights = new Weight[inputNum * hiddenNum];
		nodes = new Node[hiddenNum];
	}

	Layer(const int inputNum, const int hiddenNum, ActivationFunctions activationFunction, Weight* newWeights, Node* newNodes) :
		inputNum(inputNum), hiddenNum(hiddenNum), activationFunction(activationFunction)
	{
		weights = newWeights;
		nodes = newNodes;
	}

	~Layer() {
		delete[] weights;
		delete[] nodes;
	}

	float* forward(float *inputs) {
		for (int i = 0; i < inputNum; i++) {
			for (int j = 0; j < hiddenNum; j++) {
				nodes[j].add(weights[(i*hiddenNum)+j].weigh(inputs[i]));
			}
		}
		
		float* sum = new float[hiddenNum];

		for (int i = 0; i < hiddenNum; i++) {
			sum[i] = ActivationFunction::forward(activationFunction, nodes[i].getSum());
		}

		return sum;
	}

	Layer* copy() {
		Weight* newWeights = new Weight[inputNum * hiddenNum];
		Node* newNodes = new Node[hiddenNum];

		for (int i = 0; i < (inputNum * hiddenNum); i++) {
			newWeights[i].weight = weights[i].weight;
		}

		for (int i = 0; i < hiddenNum; i++) {
			newNodes[i].bias = nodes[i].bias;
		}

		return new Layer(inputNum, hiddenNum, activationFunction, newWeights, newNodes);
	}

	void mutate(float rate) {
		float ran = 0;

		for (int i = 0; i < (inputNum * hiddenNum); i++) {
			ran = (std::rand() % 1000) / 10.f;
			if(ran < rate)
				weights[i].weight = clip(weights[i].weight + generateFloat(-MUTATION_RANGE, MUTATION_RANGE), -2.0f, 2.0f);
		}

		for (int i = 0; i < hiddenNum; i++) {
			ran = (std::rand() % 1000) / 10.f;
			if (ran < rate)
				nodes[i].bias = clip(nodes[i].bias + generateFloat(-MUTATION_RANGE, MUTATION_RANGE), -2.0f, 2.0f);
		}
	}

	void initRandom() {
		for (int i = 0; i < (inputNum * hiddenNum); i++) {
			weights[i].weight = generateFloat(-1.0f, 1.0f);
		}

		for (int i = 0; i < hiddenNum; i++) {
			nodes[i].bias = generateFloat(-1.0f, 1.0f);
		}
	}


};

#endif