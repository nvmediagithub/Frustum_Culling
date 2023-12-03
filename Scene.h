#pragma once
#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rapidjson/document.h"

#include "ResourceManager.h"
#include "RenderManager.h"

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "GraphicObject.h"

// КЛАСС ДЛЯ РАБОТЫ СО СЦЕНОЙ
class Scene
{
public:
	// конструктор по умолчанию
	Scene();

	// Инициализация сцены
	// параметр: json-файл с описанием моделей (models.json)
	void init(std::string filename);

	// загрузка сцены из json-файла
	bool loadFromJson(std::string filename);

	// установка используемой камеры (ссылка на глобальный объект)
	void setCamera(Camera *camera);
	// установка используемого источника света (ссылка на глобальный объект)
	void setLight(Light *light);

	// Вывод всей сцены (посредством обращения к RenderManager)
	void draw();

	// Получение информации о сцене для отладки:
	// [количество объектов переданное на отрисовку] / [общее количество объектов в сцене]
	std::string getSceneDescription();

private:
	// создание объекта по его идентификатору в json-файле ("models.json")
	GraphicObject createGraphicObject(std::string model);
	
	// определение, виден ли объект в зависимости от дистанци до него
	bool lodTest(GraphicObject& graphicObject);

	// определение, попадает ли объект в усеченую пирамиду видимости
	bool frustumCullingTest(GraphicObject& graphicObject);

private:
	// json-представление всех моделей (из файла "models.json")
	rapidjson::Document document;
	// указатель на камеру (на глобальную переменную)
	Camera *camera;
	// указатель на пирамиду видимости (на глобальную переменную)
	Camera *frustumCullingCamera;
	// указатель на источник света (на глоабльную переменную)
	Light *light;
	// все графические объекты
	std::vector<GraphicObject> graphicObjects;
	// статистика - сколько объектов переданно на отрисовку в текущем кадре
	int renderedObjectCount;
};
