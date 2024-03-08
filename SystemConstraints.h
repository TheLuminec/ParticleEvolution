#pragma once
#ifndef SYSTEMCONSTRAINTS_H
#define SYSTEMCONSTRAINTS_H
#include <SFML/Graphics.hpp>
#include <unordered_map>

const float SCALE = 1.0f;
const __int16 WIDTH = 1024 / SCALE;
const __int16 HEIGHT = 1024 / SCALE;

const bool BORDERS = true;
const float COLLISON_EFFICIENCY = 0.95f;
const float WALL_COLLISON_EFFICIENCY = 0.25f;
const float STEPPING = 0.05f;
const float MUTATION_RATE = 3.0f;//percent
const float MUTATION_RANGE = 0.3f;
int generationNumber = 0;

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

float generateFloat(const float &min, const float &max) {
	return static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max-min))) + min;
}

#endif