#pragma once
#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "GraphicObject.h"
#include "Light.h"

// КЛАСС ДЛЯ ВЗАИМОДЕЙСТВИЯ С OPENGL
// ВЕСЬ ВЫВОД ОСУЩЕСТВЛЯЕТСЯ ЧЕРЕЗ ЕДИНСТВЕННЫЙ ЭКЗЕМПЛЯР ДАННОГО КЛАССА
class RenderManager
{
private:
	// конструктор по умолчанию (приватный)
	RenderManager() {};
	// конструктора копирования нет
	RenderManager(const RenderManager& root) = delete;
	// оператора присваивания нет
	RenderManager& operator=(const RenderManager&) = delete;

public:
	// Статик-метод для получения экземпляра менеджера ресурса.
	static RenderManager& instance()
	{
		static RenderManager renderManager;
		return renderManager;
	}

	// инициализация объекта RenderManager, выполняется после инициализации OpenGL:
	// загрузка шейдеров, установка неизменных параметров и прочая инициализация
	void init();

	// установка размера окна
	void setWindowSize(int w, int h);

	// Начало вывода очередного кадра (подготовка, очистка вектора графических объектов)
	void start();

	// установка используемой камеры
	void setCamera(Camera *camera);

	//
	void setFrustumCullingCamera(Camera* frustumCullingCamera);

	// установка используемого источника света
	void setLight(Light *light);
	// добавление в очередь рендеринга очередного объекта для вывода
	void addToRenderQueue(GraphicObject& graphicObject);

	// завершение вывода кадра (основная работа)
	void finish();


private:
	// установка (привязка) текстуры по её ID в менеджере ресурсов
	void setTexture(Shader& shader, int textureId);
	// установка параметров материала по его ID в менеджере ресурсов
	void setMaterial(Shader& shader, int materialId);

	// вывод всех графических объектов
	void drawAllObjects();
	// вывод пирамиды видимости
	void drawViewFrustum();

private:
	// используемые шейдеры 
	std::vector<Shader>	shaders;

	// размеры окна
	int windowWidth, windowHeight;

	// указатель на камеру
	Camera* camera;

	// указатель на камеру усеченной пирамиды
	Camera* frustumCullingCamera;
	
	// указатель на источник света
	Light* light;

	// ранее установленная текстура
	int previouseTextureID;
	// ранее установленный материал
	int previouseMaterialID;

	// режим пост-обработки
	int postProcessingMode;

	// список графических объектов, которые необходимо вывести в данном кадре
	std::vector<GraphicObject> graphicObjects;

	// массив для накопления матриц моделей при инстансинге
	std::vector<glm::mat4> instancedMvMatrices;
};
