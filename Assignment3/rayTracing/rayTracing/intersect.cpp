
#include "Intersect.h"
#include "Raytracing.h"
#include "objectPrim.h"
#include <tuple>

//Intersection for when a ray hits an objects
Intersect::Intersect(Raytray& ray, Objprim& primitive)
{
	bool hits;
	float distance;
	//std::tie(hits, distance) = primitive.intersection(ray); //pair objects together and find the intersection
	hitsObject = hits;
	distanceFromHit = distance;
}

//Intersection for a miss
Intersect::Intersect()
{
	hitsObject = false;
	distanceFromHit = 0.0f;
}


bool Intersect::getHits() { return hitsObject; }
float Intersect::getDistance(){	return distanceFromHit;}