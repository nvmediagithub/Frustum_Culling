#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ����� ��� ������ � ����������
class Material
{
public:
	// ����������� �� ���������
	Material ();

	// �������� ���������� ��������� �� json-�����
	bool loadFromJson(std::string filename);

	// ��������� ��������� ���������� ���������
	glm::vec4& getAmbient();
	glm::vec4& getDiffuse();
	glm::vec4& getSpecular();
	float getShininess();

private:
	// ������� ������������
	glm::vec4 ambient;
	// ��������� ������������
	glm::vec4 diffuse;
	// ���������� ������������
	glm::vec4 specular;
	// ������� ����������������
	GLfloat shininess;
};
