#include "Scene.h"

using namespace glm;
using namespace std;
using namespace rapidjson;


// конструктор по умолчанию
Scene::Scene()
{
	camera = nullptr;
	light = nullptr;
	renderedObjectCount = 0;
}

// Инициализация сцены (параметр: json-файл с описанием моделей)
void Scene::init(std::string filename)
{
	// открываем файл с заданным именем и проверяем, удалось ли его открыть
	ifstream f(filename);
	if (!f.is_open()) {
		cout << "Не уадется найти файл " << filename << endl;
		return;
	}

	// загружаем весь исходный текст (до разделителя - нулевого символа)
	string jsonString;
	getline(f, jsonString, static_cast<char>(0));
	f.close();

	//// выводим считанную строку для отладки
	//cout << jsonString << endl;

	// парсим - если ошибка, то выходим
	document.Parse(jsonString.c_str());
	if (document.GetParseError() != 0) {
		cout << "неверный формат файла " << filename << endl;
		return;
	}
}

// загрузка сцены из json-файла
bool Scene::loadFromJson(std::string filename)
{
	// открываем файл с заданным именем и проверяем, удалось ли его открыть
	ifstream f(filename);
	if (!f.is_open()) {
		cout << "Не уадется найти файл " << filename << endl;
		return false;
	}

	// загружаем весь исходный текст (до разделителя - нулевого символа)
	string jsonString;
	getline(f, jsonString, static_cast<char>(0));
	f.close();

	//// выводим считанную строку для отладки
	//cout << jsonString << endl;

	// парсим - если ошибка, то выходим
	Document document;
	document.Parse(jsonString.c_str());
	if (document.GetParseError() != 0) {
		cout << "wrong file format " << filename << endl;
		return false;
	}

	// перебор всех графических объектов
	for (auto& it : document.GetArray()) {
		// считываем данные об очередном графическом объекте
		string model_id = it["model"].GetString();
		vec3 position;
		position.x = it["position"][0].GetFloat();
		position.y = it["position"][1].GetFloat();
		position.z = it["position"][2].GetFloat();
		float angle = it["angle"].GetFloat();
		
		//// контрольный вывод
		//cout << model_id << endl;
		//cout << position.x << " " << position.y << " " << position.z << endl;
		//cout << angle << endl;

		// формируем графический объект
		auto grObj = createGraphicObject(model_id);
		grObj.setPosition(position);
		grObj.setAngle(angle);
		graphicObjects.push_back(grObj);
	}

	return true;
}

// установка используемой камеры (ссылка на глобальный объект)
void Scene::setCamera(Camera *camera)
{
	this->camera = camera;
}

// установка используемого источника света (ссылка на глобальный объект)
void Scene::setLight(Light *light)
{
	this->light = light;
}

// Вывод всей сцены (посредством обращения к RenderManager)
void Scene::draw()
{
	// начинаем вывод сцены в новом кадре
	renderedObjectCount = 0;

	// добавляем в очередь все объекты, которые необходимо вывести с учетом оптимизации
	for (auto& grObj : graphicObjects) {
		// если объект не прошел тест на уровень детализации, то его не выводим
		if (!lodTest(grObj)) continue;
		// если объект не прошел frustum culling, то его не выводим
		if (!frustumCullingTest(grObj)) continue;
		// иначе добавляем объект в очередь на отрисовку
		renderedObjectCount++;
		RenderManager::instance().addToRenderQueue(grObj);
	}
}

// Получение информации о сцене для отладки:
// [количество объектов переданное на отрисовку] / [общее количество объектов]
std::string Scene::getSceneDescription()
{
	char temp[256];
	sprintf(temp, "%i/%i objects rendered", renderedObjectCount, graphicObjects.size());
	return temp;
}

// создание объекта по его идентификатору в json-файле ("models.json")
GraphicObject Scene::createGraphicObject(std::string model)
{
	// получаем все данные
	auto modelDescr = document[model.c_str()].GetObjectW();

	string meshFileName = modelDescr["mesh"].GetString();
	string textureFileName = modelDescr["texture"].GetString();
	string materialFileName = modelDescr["material"].GetString();
	string type = modelDescr["type"].GetString();
	vec3 dimensions;
	dimensions.x = modelDescr["dimensions"][0].GetFloat();
	dimensions.y = modelDescr["dimensions"][1].GetFloat();
	dimensions.z = modelDescr["dimensions"][2].GetFloat();

	// создаем графический объект
	ResourceManager &rm = ResourceManager::instance();
	GraphicObject grObj;

	grObj.setMeshId(rm.loadMesh(meshFileName));
	grObj.setTextureId(rm.loadTexture(textureFileName));
	grObj.setMaterialId(rm.loadMaterial(materialFileName));
	grObj.setDimensions(dimensions);
	if (type == "road") grObj.setType(GraphicObjectType::road);
	else if (type == "building") grObj.setType(GraphicObjectType::building);
	else if (type == "vehicle") grObj.setType(GraphicObjectType::vehicle);
	else if (type == "big nature") grObj.setType(GraphicObjectType::big_nature);
	else if (type == "small nature") grObj.setType(GraphicObjectType::small_nature);
	else if (type == "big prop") grObj.setType(GraphicObjectType::big_prop);
	else if (type == "medium prop") grObj.setType(GraphicObjectType::medium_prop);
	else if (type == "small prop") grObj.setType(GraphicObjectType::small_prop);

	// возвращаем созданный объект
	return grObj;
}

// определение, виден ли объект в зависимости от дистанци до него
bool Scene::lodTest(GraphicObject& graphicObject)
{
	float distance = glm::distance(camera->getPosition(), graphicObject.getPosition());

	static const float max_distance[] = { 1200, 1200, 1200, 500, 350.0, 200.0, 400.0, 300.0, 200.0 };

	if (distance < max_distance[static_cast<int>(graphicObject.getType())]) return true;

	return false;
}

// определение, попадает ли объект в усеченую пирамиду видимости
bool Scene::frustumCullingTest(GraphicObject& graphicObject)
{
	// получаем все вершины ограничивающего объема
	vec3 dimensions = graphicObject.getDimensions();
	vector<vec4> OBB = {
		vec4(+dimensions.x / 2, +dimensions.y / 2, +dimensions.z / 2, 1.0),
		vec4(+dimensions.x / 2, +dimensions.y / 2, -dimensions.z / 2, 1.0),
		vec4(+dimensions.x / 2, -dimensions.y / 2, +dimensions.z / 2, 1.0),
		vec4(+dimensions.x / 2, -dimensions.y / 2, -dimensions.z / 2, 1.0),
		vec4(-dimensions.x / 2, +dimensions.y / 2, +dimensions.z / 2, 1.0),
		vec4(-dimensions.x / 2, +dimensions.y / 2, -dimensions.z / 2, 1.0),
		vec4(-dimensions.x / 2, -dimensions.y / 2, +dimensions.z / 2, 1.0),
		vec4(-dimensions.x / 2, -dimensions.y / 2, -dimensions.z / 2, 1.0),
	};

	// матрица преобразовани¤ в усеченную систему координат (clip space)
	mat4 PVM = camera->getProjectionMatrix() * camera->getViewMatrix() * graphicObject.getModelMatrix();

	// проверяем расположение всех вершин относительно всех 6 плоскостей frustum'а
	int flags[6]{ 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 8; i++) {
		vec4 b = PVM * OBB[i];
		if (b.x < -b.w) flags[0]++;
		if (b.x > +b.w) flags[1]++;
		if (b.y < -b.w) flags[2]++;
		if (b.y > +b.w) flags[3]++;
		if (b.z < -b.w) flags[4]++;
		if (b.z > +b.w) flags[5]++;
	};

	// определяем была ли видна хотя бы одна вершина
	for (int i = 0; i < 6; i++) {
		if (flags[i] >= 8) return false;
	}

	return true;
}