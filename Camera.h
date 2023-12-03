#pragma once

#include <windows.h>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// КЛАСС ДЛЯ РАБОТЫ С КАМЕРОЙ
class Camera
{
public:
	// конструктор по умолчанию
	Camera();

	// установить матрицу проекции
	void setProjectionMatrix(float	fovy, float aspect, float zNear, float zFar);
	// получить матрицу проекции
	glm::mat4& getProjectionMatrix();

	// получить матрицу вида
	glm::mat4& getViewMatrix();

	// передвинуть камеру и точку наблюдения в горизонтальной плоскости (OXZ)
	void moveOXZ(float dx, float dz);
	// повернуть в горизонтальной и вертикальной плоскости (угол задается в градусах)
	void rotate(float horizAngle, float vertAngle);
	// приблизить/удалить камеру к/от точки наблюдения
	void zoom(float dR);

	// получить позицию камеры
	glm::vec3 getPosition();

private:
	// пересчитать матрицу вида
	void recalculateViewMatrix();

private:
	// радиус и углы поворота (для вычисления положения камеры в сферических координатах)
	float r;
	float angleX;
	float angleY;

	// текущяя позиция камеры
	glm::vec3 position;
	// точка, в которую направленна камера
	glm::vec3 center;

	// матрица проекции
	glm::mat4 projectionMatrix;
	// матрица наблюдения 
	glm::mat4 viewMatrix;
};