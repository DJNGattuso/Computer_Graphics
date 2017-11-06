#pragma once
#ifndef INTERSECT_H
#define INTERSECT_H

#include "Raytracing.h"
#include "objectPrim.h"

class Intersect
{
public:
	Intersect(Raytray& ray, Objprim& primitive);
	Intersect();

	//------Getters-----
	bool getHits();
	float getDistance();

private:
	bool hitsObject;
	float distanceFromHit;
};

#endif