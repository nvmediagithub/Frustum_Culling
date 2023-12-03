#include "Light.h"

using namespace	glm;

Light::Light()
{
	direction = vec4(0.3, 1.0, 0.5, 0);
	ambient = vec4(1.0, 1.0, 1.0, 1);
	diffuse = vec4(0.6, 0.6, 0.6, 1);
	specular = vec4(0.4, 0.4, 0.4, 1);
};

void Light::setDirection(vec3 &direction)
{
	this->direction = vec4(direction, 0.0);
}

void Light::setAmbient(vec4 &ambient)
{
	this->ambient = ambient;
}

void Light::setDiffuse(vec4 &diffuse) 
{
	this->diffuse = diffuse;
}

void Light::setSpecular(vec4 &specular)
{
	this->specular = specular;
}

vec4& Light::getDirection()
{
	return direction;
}

vec4& Light::getAmbient()
{
	return ambient;
}

vec4& Light::getDiffuse()
{
	return diffuse;
}

vec4& Light::getSpecular()
{
	return specular;
}

