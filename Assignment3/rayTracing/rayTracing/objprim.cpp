
#include "objectPrim.h"


Objprim::Objprim(const glm::vec3 ambColour, const glm::vec3 difColour, const glm::vec3 specColour, const float shine)
{
	setAmbientColour(ambColour);
	setDiffuseColour(difColour);
	setSpecularColour(specColour);
	setShininess(shine);
}

//default constructor
Objprim::Objprim()
	: ambient(glm::vec3(0.f, 0.f, 0.f))
	, diffuse(glm::vec3(0.f, 0.f, 0.f))
	, specular(glm::vec3(0.f, 0.f, 0.f))
	, shine(0.f) {}


//-----------------------------------------Getters-----------------------------------
glm::vec3 Objprim::getAmbient() const { return ambient; }
glm::vec3 Objprim::getDiffuse() const { return diffuse; }
glm::vec3 Objprim::getSpecular() const { return specular; }
float Objprim::getShininess() const { return shine; }

//-----------------------------------------Setters------------------------------------
void Objprim::setAmbientColour(const glm::vec3 ambientColour)
{
	ambient = ambientColour;
}
void Objprim::setDiffuseColour(const glm::vec3 diffuseColour)
{
	diffuse = diffuseColour;
}
void Objprim::setSpecularColour(const glm::vec3 specularColour)
{
	specular = specularColour;
}
void Objprim::setShininess(const float shininess)
{
	shine = shininess;
}
