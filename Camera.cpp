#include "camera.h"

using	namespace glm;

#define M_PI 3.14159265358979323846
const float	X_Speed = 90.0;
const float	Y_Speed = 40.0;


// конструкор по умолчанию
Camera::Camera()
{
	r = 75;
	angleX = 90.5;
	angleY = 26;
	center = vec3(-0.35, 0.0, 4.12);

	// пересчитать матрицу вида
	recalculateViewMatrix();
}

// установка матрицы проекции
void Camera::setProjectionMatrix (float	fovy, float aspect, float zNear, float zFar)
{
	projectionMatrix = perspective (radians(fovy), aspect, zNear, zFar);
}

// получение матрицы проекции
mat4& Camera::getProjectionMatrix ()
{
	return projectionMatrix;
}

// получить матрицу камеры (наблюдения)
mat4& Camera::getViewMatrix()
{
	return viewMatrix;
}

// пересчитать матрицу вида
void Camera::recalculateViewMatrix()
{
	// находим вектор смещения
	vec3 offset;
	offset.y = r * sin(radians(angleY));
	float	rx = r * cos(radians(angleY));
	offset.x = rx * cos(radians(angleX));
	offset.z = rx * sin(radians(angleX));
	// находим новую позицию
	position = center + offset;
	// вычисляем матрицу камеры
	viewMatrix = lookAt(position, center, vec3(0, 1, 0));
}

// передвинуть камеру и точку наблюдения в горизонатльной плоскости oXZ
void Camera::moveOXZ(float dx, float dz) 
{
	vec3 cameraDirection = center - position;
	cameraDirection.y = 0;
	cameraDirection = normalize(cameraDirection);
	vec3 rightDirection = cross(cameraDirection, vec3(0,1,0));
	vec3 offset = cameraDirection*dz - rightDirection*dx;
	position = position + offset;
	center = center + offset;
	
	// пересчитать матрицу вида
	recalculateViewMatrix();
}

// вращение в горизонтальной и вертикальной плоскости
void Camera::rotate(float horiz, float vert)
{
	angleX += horiz;
	if (angleX > 360) angleX -= 360;
	if (angleX < 0) angleX += 360;
	angleY += vert;
	if (angleY > 85) angleY = 85;
	if (angleY < 5) angleY = 5;

	// пересчитать матрицу вида
	recalculateViewMatrix();
}

// приближение/удаление
void Camera::zoom(float dR)
{
	r += dR;
	if (r < 3.0) r = 3.0;
	if (r > 500.0) r = 500.0;

	// пересчитать матрицу вида
	recalculateViewMatrix();
}

// получить позицию камеры
glm::vec3 Camera::getPosition()
{
	return position;
}

