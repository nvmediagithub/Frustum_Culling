#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstddef>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ����� ��� ������ � �����
class Mesh
{
public:
	// �����������
	Mesh();
	// �������� ���� �� �������� obj-�����
	bool load(std::string filename);
	// ����� ����
	void drawOne();
	// ����� ����������������� ���� (������� count �����) 
	void drawMany(int count);

private:
	// ������ VAO-�������
	GLuint vao;		
	// ������ VBO-������ ������
	GLuint vertexBuffer;
	// ������ VBO-������ ��������
	GLuint indexBuffer;
	// ���������� ������ � ����
	int vertexCount;	
};