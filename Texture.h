#pragma once

#include <windows.h>
#include <iostream>
#include <string>

#include "glew.h"
#include "GL/freeglut.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

// ����� ��� ������ � ���������
class Texture
{
public:
	// �������� �������� �� �������� �����
	void load (std::string filename);
	// ���������� �������� (�������� � ����������� �����)
	void bind (GLenum texUnit = GL_TEXTURE0);

private:
	// ������ ����������� �������
	GLuint texIndex;
};