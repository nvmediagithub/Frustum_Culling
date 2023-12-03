#include <algorithm>

#include "RenderManager.h"

using namespace glm;
using namespace std;


// инициализация объекта RenderManager, выоплняется после инициализации OpenGL:
// загрузка шейдеров и т.д.
void RenderManager::init()
{
	// загрузка шейдера
	shaders.emplace_back();
	shaders[0].load("DATA\\SHADERS\\DiffuseTexture.vsh", "DATA\\SHADERS\\DiffuseTexture.fsh");

	shaders.emplace_back();
	shaders[1].load("DATA\\SHADERS\\ViewingFrustum.vsh", "DATA\\SHADERS\\ViewingFrustum.fsh");


	// сбрасываем ранее установленные текстуры и материал
	previouseTextureID = -1;
	previouseMaterialID = -1;
}


// Начало вывода очередного кадра
void RenderManager::start()
{
	// удаляем все ранее установленные елементы массива GraphicObjects
	graphicObjects.clear();
	// сбрасываем ранее установленные текстуры и материал
	previouseTextureID = -1;
	previouseMaterialID = -1;
}

// установка используемой камеры
void RenderManager::setCamera(Camera* сamera)
{
	this->camera = сamera;
}

void RenderManager::setFrustumCullingCamera(Camera* frustumCullingCamera)
{
	this->frustumCullingCamera = frustumCullingCamera;
}

// установка используемого источника света
void RenderManager::setLight(Light *light)
{
	this->light = light;
}

// добавление в очередь рендеринга очередного объекта для вывода
void RenderManager::addToRenderQueue(GraphicObject& graphicObject)
{
	graphicObjects.push_back(graphicObject);
	instancedMvMatrices.clear();
}

// завершение вывода кадра
void RenderManager::finish()
{
	// отчищаем буфер кадра
	glClearColor(0.880, 0.890, 0.910, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// устанавливаем некоторые дополнительные параметры рендеринга
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// выводим все объекты
	drawAllObjects();

	// вывод пирамиды видимости
	shaders[1].activate();
	mat4 projectionMatrix = camera->getProjectionMatrix();
	mat4 viewMatrix = camera->getViewMatrix();
	shaders[1].setUniform("projectionMatrix", projectionMatrix);
	mat4 modelMatrix = mat4(1.0f);
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	shaders[1].setUniform("modelViewMatrix", modelViewMatrix);
	drawViewFrustum();
}

// установка размера окна
void RenderManager::setWindowSize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}


// установка (привязка) текстуры по её ID в менеджере ресурсов
void RenderManager::setTexture(Shader& shader, int textureId)
{
	if (previouseTextureID == textureId) return;
	previouseTextureID = textureId;

	Texture *texture = ResourceManager::instance().getTexture(textureId);
	if (texture != nullptr) texture->bind();
	shader.setUniform("texture_0", 0);
}

// установка параметров материала по его ID в менеджере ресурсов
void RenderManager::setMaterial(Shader& shader, int materialId)
{
	if (previouseMaterialID == materialId) return;
	previouseMaterialID = materialId;

	Material *material = ResourceManager::instance().getMaterial(materialId);
	if (material != nullptr) {
		shader.setUniform("mAmbient", material->getAmbient());
		shader.setUniform("mDiffuse", material->getDiffuse());
		shader.setUniform("mSpecular", material->getSpecular());
		shader.setUniform("mShininess", material->getShininess());
	}
}


// вывод всех графических объектов
void RenderManager::drawAllObjects()
{
	// предварительная сортировка по материалам
	sort(graphicObjects.begin(), graphicObjects.end(), [](GraphicObject &a, GraphicObject &b) {
		auto m1 = a.getMaterialId();
		auto m2 = b.getMaterialId();
		auto t1 = a.getTextureId();
		auto t2 = b.getTextureId();
		auto g1 = a.getMeshId();
		auto g2 = b.getMeshId();
		if (m1 != m2) return m1 < m2;
		if (t1 != t2) return t1 < t2;
		return g1 < g2;
	});

	// активируем шейдер, используемый для вывода объекта
	Shader& shader = shaders[0];
	shader = shaders[0];
	shader.activate();

	// устанавливаем матрицу проекции
	mat4& projectionMatrix = camera->getProjectionMatrix();
	shader.setUniform("projectionMatrix", projectionMatrix);

	// получаем матрицу камеры
	mat4& viewMatrix = camera->getViewMatrix();

	// устанавливаем параметры источника света
	// они являются общими для всех объектов
	shader.setUniform("lAmbient", light->getAmbient());
	shader.setUniform("lDiffuse", light->getDiffuse());
	shader.setUniform("lSpecular", light->getSpecular());
	// при установке направления на источник света
	// его необходимо преобразовать в систему координат наблюдателя
	shader.setUniform("lPosition", viewMatrix * light->getDirection());

	// сбрасываем накопленные матрицы наблюдения модели
	instancedMvMatrices.clear();

	// выводим все объекты
	for (int i = 0; i < graphicObjects.size(); i++) {

		// устанавливаем текстуру
		int textureId = graphicObjects[i].getTextureId();
		setTexture(shader, textureId);

		// устанавливаем параметры материала
		int materialId = graphicObjects[i].getMaterialId();
		setMaterial(shader, materialId);

		// накапливаем матрицы наблюдения модели
		mat4 modelViewMatrix = viewMatrix * graphicObjects[i].getModelMatrix();
		instancedMvMatrices.push_back(modelViewMatrix);

		// если необходимо вывести инстанцированные модели, то выводим их

		int count = instancedMvMatrices.size();
		// устанавливаем все матрицы			
		shader.setUniform("modelViewMatrix", modelViewMatrix);

		// выводим инстанцированный меш
		int meshId = graphicObjects[i].getMeshId();
		Mesh *mesh = ResourceManager::instance().getMesh(meshId);
		if (mesh != nullptr) {
			mesh->drawMany(count);
		};
		// завершаем вывод инстанцированного меша
		instancedMvMatrices.clear();

	}
}


// функция вывода усеченной пирамиды видимости
void RenderManager::drawViewFrustum()
{
	mat4& projectionMatrix = frustumCullingCamera->getProjectionMatrix();
	mat4& viewMatrix = frustumCullingCamera->getViewMatrix();

	glm::vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	glm::vec3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;

	// Нормализованные устройственные координаты для углов ближней и дальней плоскостей
	glm::vec4 corners[8] = {
		glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), // Левый нижний передний угол
		glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),  // Правый нижний передний угол
		glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),  // Левый верхний передний угол
		glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),   // Правый верхний передний угол
		glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),  // Левый нижний задний угол
		glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),   // Правый нижний задний угол
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),   // Левый верхний задний угол
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)     // Правый верхний задний угол
	};

	// Умножаем углы на обратную матрицу проекции-вида
	glm::mat4 inverseVP = glm::inverse(projectionMatrix * viewMatrix);

	for (int i = 0; i < 8; ++i) {
		glm::vec4 worldCoords = inverseVP * corners[i];

		// Делим на W, чтобы получить нормализованные мировые координаты
		worldCoords /= worldCoords.w;

		// Размещаем результаты в массивах координат 
		if (i == 0) nearBottomLeft = glm::vec3(worldCoords);
		else if (i == 1) nearBottomRight = glm::vec3(worldCoords);
		else if (i == 2) nearTopLeft = glm::vec3(worldCoords);
		else if (i == 3) nearTopRight = glm::vec3(worldCoords);
		else if (i == 4) farBottomLeft = glm::vec3(worldCoords);
		else if (i == 5) farBottomRight = glm::vec3(worldCoords);
		else if (i == 6) farTopLeft = glm::vec3(worldCoords);
		else if (i == 7) farTopRight = glm::vec3(worldCoords);
	}

	GLfloat	verteces[32] = {
	nearTopLeft.x,
	nearTopLeft.y,
	nearTopLeft.z,
	farTopLeft.x,
	farTopLeft.y,
	farTopLeft.z,

	nearTopRight.x,
	nearTopRight.y,
	nearTopRight.z,
	farTopRight.x,
	farTopRight.y,
	farTopRight.z,

	nearBottomLeft.x,
	nearBottomLeft.y,
	nearBottomLeft.z,
	farBottomLeft.x,
	farBottomLeft.y,
	farBottomLeft.z,

	nearBottomRight.x,
	nearBottomRight.y,
	nearBottomRight.z,
	farBottomRight.x,
	farBottomRight.y,
	farBottomRight.z,
	};

	// переменные дл¤ вывода объекта
	static bool init = true;
	static GLuint vao = 0;				// индекс VAO-буфера
	static GLuint vertexBuffer = 0;		// индекс VBO-буфера
	static int vertexCount = 0;			// количество вершин

	// при первом вызове инициализируем VBO и VAO
	if (init) {
		init = false;


		// ЗАПОЛНЕНИЕ БУФЕРОВ VBO
		// создание и привязка VAO-объекта
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		// создаем и заполняем буфер вершин
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STREAM_DRAW);

		// определяем количество вершин (индексов)
		vertexCount = 8;
		// заполнение VAO
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// "отвязка" буфера VAO, чтоб случайно не испортить
		glBindVertexArray(0);
	}

	// выводим
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STREAM_DRAW);
	glDrawArrays(GL_LINES, 0, vertexCount);
	glBindVertexArray(0);
	return;
}