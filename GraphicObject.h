#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ТИП ДАННЫХ ГРАФИЧЕСКОГО ОБЪЕКТА
enum class GraphicObjectType { none, road, building, vehicle, 
	big_nature, small_nature, big_prop, medium_prop, small_prop };


// КЛАСС ДЛЯ РАБОТЫ С ГРАФИЧЕСКИМ ОБЪЕКТОМ
class GraphicObject
{
public:
	// конструктор по умолчанию
	GraphicObject();

	// установить различные параметры
	void setColor(glm::vec4 &color);
	void setPosition(glm::vec3 &position);
	void setAngle(float degree);
	void setType(GraphicObjectType type);
	void setDimensions(glm::vec3 &dimensions);

	// установить идентификаторы ресурсов
	void setMeshId(int id);
	void setTextureId(int id);
	void setMaterialId(int id);

	// получить различные параметры 
	glm::vec4 getColor();
	glm::vec3 getPosition();
	float getAngle();
	GraphicObjectType getType();
	glm::vec3 getDimensions();

	// получить идентификаторы ресурсов
	int getMeshId();
	int getTextureId();
	int getMaterialId();

	// получить матрицу модели
	glm::mat4& getModelMatrix();

private:
	// цвет объекта
	glm::vec4 color;
	// тип модели
	GraphicObjectType type;
	// размер модели (W x H x D)
	glm::vec3 dimensions;
	// позиция
	glm::vec3 position;
	// угол поворота в горизонтальной плоскости в градусах, по часовой стрелке
	float angle;

	// матрица модели (задает позицию и ориентацию)
	glm::mat4 modelMatrix;

	// идентификатор используемого меша
	int meshId;
	// идентификатор используемой текстуры
	int textureId;
	// идентификатор используемого материала
	int materialId;
};
