#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// КЛАСС ДЛЯ РАБОТЫ С ИСТОЧНИКОМ СВЕТА
class Light
{
public:
	// конструктор по умолчанию
	Light ();

	// задание различных параметров источника света
	void setDirection(glm::vec3& direction);
	void setAmbient(glm::vec4& ambient);
	void setDiffuse(glm::vec4& diffuse);
	void setSpecular(glm::vec4& specular);

	// получение различных параметров источника света
	glm::vec4& getDirection();
	glm::vec4& getAmbient();
	glm::vec4& getDiffuse();
	glm::vec4& getSpecular();

private:
	// позиция источника света
	glm::vec4 direction;
	// фоновая составляющая источника света
	glm::vec4 ambient;
	// диффузная составляющая
	glm::vec4 diffuse;
	// зеркальная составляющая
	glm::vec4 specular;
};