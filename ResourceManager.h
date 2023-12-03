#pragma once

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

// КЛАСС ДЛЯ РАБОТЫ С МЕНЕДЖЕРОМ РЕСУРСОВ (РЕАЛИЗОВАН НА ОСНОВЕ ШАБЛОНА SINGLTON)
class ResourceManager 
{
public:
	// Статик-метод для получения экземпляра менеджера ресурса.
	// Всегда будет возвращена ссылка на объект, хранящийся в единственном экземпляре.
	static ResourceManager& instance()
	{
		static ResourceManager ResourceManager;
		return ResourceManager;
	}

    // Загрузка одного меша из указанного obj-файла
    // Возвращаемое значение - индекс меша в менеджере ресурсов
	int loadMesh(std::string filename);

    // Получение меша по его индексу
    // Если такого меша нет (недействительный индекс) возвращается nullptr
	Mesh* getMesh(int index);

	// Загрузка одной текстуры.
	// Возвращаемое значение - индекс текстуры в менеджере ресурсов.
	int loadTexture(std::string filename);

	// Получение текстуры по её индексу.
	// Если такой текстуры нет (недействительный индекс) возвращается nullptr
	Texture* getTexture(int index);

	// Загрузка одного материала
	// Возвращаемое значение - индекс материала в менеджере ресурсов.
	int loadMaterial(std::string filename);

	// Получение текстуры по её индексу.
	// Если такой текстуры нет (недействительный индекс) возвращается nullptr
	Material* getMaterial(int index);

private:
	// конструктор по умолчанию (объявлен приватным)
	// в результате нельзя создать ни одного объекта данного класса вне самого класса
	ResourceManager() {};
	// конструктора копирования нет (удален)
	ResourceManager(const ResourceManager& v) = delete;
	// оператора присваивания нет (удален)
	ResourceManager& operator=(const ResourceManager& v) = delete;

private:
	// вектор для хранения всех мешей
	std::vector<Mesh> meshes;
	// map для хранения соответствия между именем запрашиваемого файла 
	// и индексом в контейнере meshes 
	std::map <std::string, int> meshes_id;

	// вектор для хранения всех текстур
	std::vector<Texture> textures;
	// map для хранения соответствия между именем запрашиваемого файла 
	// и индексом в контейнере textures 
	std::map <std::string, int> textures_id;

	// вектор для хранения всех материалов
	std::vector<Material> materials;
	// map для хранения соответствия между именем запрашиваемого файла 
	// и индексом в контейнере materials 
	std::map <std::string, int> materials_id;
};
