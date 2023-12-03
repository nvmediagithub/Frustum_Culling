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

// КЛАСС ДЛЯ РАБОТЫ С МЕШЕМ
class Mesh
{
public:
	// конструктор
	Mesh();
	// загрузка меша из внешнего obj-файла
	bool load(std::string filename);
	// вывод меша
	void drawOne();
	// вывод инстанцированного меша (вывести count мешей) 
	void drawMany(int count);

private:
	// индекс VAO-объекта
	GLuint vao;		
	// индекс VBO-буфера вершин
	GLuint vertexBuffer;
	// индекс VBO-буфера индексов
	GLuint indexBuffer;
	// количество вершин в меше
	int vertexCount;	
};