#pragma once

#include <windows.h>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ����� ��� ������ � �������
class Camera
{
public:
	// ����������� �� ���������
	Camera();

	// ���������� ������� ��������
	void setProjectionMatrix(float	fovy, float aspect, float zNear, float zFar);
	// �������� ������� ��������
	glm::mat4& getProjectionMatrix();

	// �������� ������� ����
	glm::mat4& getViewMatrix();

	// ����������� ������ � ����� ���������� � �������������� ��������� (OXZ)
	void moveOXZ(float dx, float dz);
	// ��������� � �������������� � ������������ ��������� (���� �������� � ��������)
	void rotate(float horizAngle, float vertAngle);
	// ����������/������� ������ �/�� ����� ����������
	void zoom(float dR);

	// �������� ������� ������
	glm::vec3 getPosition();

private:
	// ����������� ������� ����
	void recalculateViewMatrix();

private:
	// ������ � ���� �������� (��� ���������� ��������� ������ � ����������� �����������)
	float r;
	float angleX;
	float angleY;

	// ������� ������� ������
	glm::vec3 position;
	// �����, � ������� ����������� ������
	glm::vec3 center;

	// ������� ��������
	glm::mat4 projectionMatrix;
	// ������� ���������� 
	glm::mat4 viewMatrix;
};