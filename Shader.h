#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ����� ��� ������ � ��������
class Shader
{
public:
	// �������� ������� �� ������� ������
	bool load(std::string veftexShaderFilename, std::string fragmentShaderFilename);

	// ����� ������� � �������� ��������
	void activate();

	// ���������� �������
	static void	deactivate();

	// ��������� uniform-����������
	void setUniform(std::string name, int value);
	void setUniform(std::string name, float value);
	void setUniform(std::string name, glm::vec2 &value);
	void setUniform(std::string name, glm::vec4 &value);
	void setUniform(std::string name, glm::mat4 &value);
	void setUniform(std::string name, std::vector<glm::mat4> &value);

private:
	// �������� ���������� ������� ���������� ���� � �������� ��������� ������ ������� �� ���������� �����
	GLuint createShaderObject(GLenum shaderType, std::string filename);

	// ��������� ������� (location) uniform-����������
	GLuint getUniformLocation(std::string name);

private:
	// id "�������" ��������� ��������� ���������
	// ������������ ����� �������� �� ������������ ��������� ���������, ���� ��� ��� ������������
	static GLuint currentProgram;

	// ��������� ��������� (������)
	GLuint	program;

	// "����������" uniform-����������, ������� ���������� ����� ����������� ������������
	// ����� ������ uniform-���������� � � �������� (location)
	std::map <std::string, GLuint> uniforms;
};