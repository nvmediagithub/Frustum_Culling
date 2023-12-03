#include "GraphicObject.h"

using namespace glm;

// КОНСТРУКТОР ПО УМОЛЧАНИЮ
GraphicObject::GraphicObject()
{
	color = vec4(1, 0, 0, 1);
	type = GraphicObjectType::none;
	dimensions = vec3(0, 0, 0);
	position = vec3(0, 0, 0);
	angle = 0.0;

	meshId = -1;
	textureId = -1;
	materialId = -1;

	modelMatrix = mat4(
		vec4(1, 0, 0, 0),		// 1-ый столбец: направление оси ox
		vec4(0, 1, 0, 0),		// 2-ой столбец: направление оси oy
		vec4(0, 0, 1, 0),		// 3-ий столбец: направление оси oz
		vec4(0, 0, 0, 1));		// 4-ый столбец: позиция объекта (начала координат)
}

// УСТАНОВИТЬ РАЗЛИЧНЫЕ ПАРАМЕТРЫ
void GraphicObject::setColor(glm::vec4 &color)
{
	this->color = color;
}

void GraphicObject::setPosition(glm::vec3 &position)
{
	this->position = position;
	modelMatrix[3] = vec4(position, 1.0);
}

void GraphicObject::setAngle(float degree)
{
	modelMatrix[0] = vec4(cos(radians(degree)), 0, sin(radians(degree)), 0);
	modelMatrix[1] = vec4(0, 1, 0, 0);
	modelMatrix[2] = vec4(-sin(radians(degree)), 0, cos(radians(degree)), 0);
}

void GraphicObject::setType(GraphicObjectType type)
{
	this->type = type;
}

void GraphicObject::setDimensions(glm::vec3 &dimensions)
{
	this->dimensions = dimensions;
}

// УСТАНОВИТЬ ИДЕНТИФИКАТОРЫ РЕСУРСОВ
void GraphicObject::setMeshId(int id)
{
    this->meshId = id;
}

void GraphicObject::setTextureId(int id)
{
	this->textureId = id;
}

void GraphicObject::setMaterialId(int id)
{
	this->materialId = id;
}

// ПОЛУЧИТЬ РАЗЛИЧНЫЕ ПАРАМЕТРЫ 
glm::vec4 GraphicObject::getColor()
{
	return color;
}

glm::vec3 GraphicObject::getPosition()
{
	return position;
}

float GraphicObject::getAngle()
{
	return angle;
}

GraphicObjectType GraphicObject::getType()
{
	return type;
}

glm::vec3 GraphicObject::getDimensions()
{
	return dimensions;
}

// ПОЛУЧИТЬ ИДЕНТИФИКАТОРЫ РЕСУРСОВ
int GraphicObject::getMeshId()
{
    return meshId;
}


int GraphicObject::getTextureId()
{
	return textureId;
}


int GraphicObject::getMaterialId()
{
	return materialId;
}

// ПОЛУЧИТЬ МАТРИЦУ МОДЕЛИ
glm::mat4& GraphicObject::getModelMatrix()
{
	return modelMatrix;
}

