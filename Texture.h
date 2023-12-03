#pragma once

#include <windows.h>
#include <iostream>
#include <string>

#include "glew.h"
#include "GL/freeglut.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

// КЛАСС ДЛЯ РАБОТЫ С ТЕКСТУРОЙ
class Texture
{
public:
	// загрузка текстуры из внешнего файла
	void load (std::string filename);
	// применение текстуры (привязка к текстурному блоку)
	void bind (GLenum texUnit = GL_TEXTURE0);

private:
	// индекс текстурного объекта
	GLuint texIndex;
};