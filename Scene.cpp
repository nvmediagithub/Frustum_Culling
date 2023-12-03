#include "Scene.h"

using namespace glm;
using namespace std;
using namespace rapidjson;


// ����������� �� ���������
Scene::Scene()
{
	camera = nullptr;
	light = nullptr;
	renderedObjectCount = 0;
}

// ������������� ����� (��������: json-���� � ��������� �������)
void Scene::init(std::string filename)
{
	// ��������� ���� � �������� ������ � ���������, ������� �� ��� �������
	ifstream f(filename);
	if (!f.is_open()) {
		cout << "�� ������� ����� ���� " << filename << endl;
		return;
	}

	// ��������� ���� �������� ����� (�� ����������� - �������� �������)
	string jsonString;
	getline(f, jsonString, static_cast<char>(0));
	f.close();

	//// ������� ��������� ������ ��� �������
	//cout << jsonString << endl;

	// ������ - ���� ������, �� �������
	document.Parse(jsonString.c_str());
	if (document.GetParseError() != 0) {
		cout << "�������� ������ ����� " << filename << endl;
		return;
	}
}

// �������� ����� �� json-�����
bool Scene::loadFromJson(std::string filename)
{
	// ��������� ���� � �������� ������ � ���������, ������� �� ��� �������
	ifstream f(filename);
	if (!f.is_open()) {
		cout << "�� ������� ����� ���� " << filename << endl;
		return false;
	}

	// ��������� ���� �������� ����� (�� ����������� - �������� �������)
	string jsonString;
	getline(f, jsonString, static_cast<char>(0));
	f.close();

	//// ������� ��������� ������ ��� �������
	//cout << jsonString << endl;

	// ������ - ���� ������, �� �������
	Document document;
	document.Parse(jsonString.c_str());
	if (document.GetParseError() != 0) {
		cout << "wrong file format " << filename << endl;
		return false;
	}

	// ������� ���� ����������� ��������
	for (auto& it : document.GetArray()) {
		// ��������� ������ �� ��������� ����������� �������
		string model_id = it["model"].GetString();
		vec3 position;
		position.x = it["position"][0].GetFloat();
		position.y = it["position"][1].GetFloat();
		position.z = it["position"][2].GetFloat();
		float angle = it["angle"].GetFloat();
		
		//// ����������� �����
		//cout << model_id << endl;
		//cout << position.x << " " << position.y << " " << position.z << endl;
		//cout << angle << endl;

		// ��������� ����������� ������
		auto grObj = createGraphicObject(model_id);
		grObj.setPosition(position);
		grObj.setAngle(angle);
		graphicObjects.push_back(grObj);
	}

	return true;
}

// ��������� ������������ ������ (������ �� ���������� ������)
void Scene::setCamera(Camera *camera)
{
	this->camera = camera;
}

// ��������� ������������� ��������� ����� (������ �� ���������� ������)
void Scene::setLight(Light *light)
{
	this->light = light;
}

// ����� ���� ����� (����������� ��������� � RenderManager)
void Scene::draw()
{
	// �������� ����� ����� � ����� �����
	renderedObjectCount = 0;

	// ��������� � ������� ��� �������, ������� ���������� ������� � ������ �����������
	for (auto& grObj : graphicObjects) {
		// ���� ������ �� ������ ���� �� ������� �����������, �� ��� �� �������
		if (!lodTest(grObj)) continue;
		// ���� ������ �� ������ frustum culling, �� ��� �� �������
		if (!frustumCullingTest(grObj)) continue;
		// ����� ��������� ������ � ������� �� ���������
		renderedObjectCount++;
		RenderManager::instance().addToRenderQueue(grObj);
	}
}

// ��������� ���������� � ����� ��� �������:
// [���������� �������� ���������� �� ���������] / [����� ���������� ��������]
std::string Scene::getSceneDescription()
{
	char temp[256];
	sprintf(temp, "%i/%i objects rendered", renderedObjectCount, graphicObjects.size());
	return temp;
}

// �������� ������� �� ��� �������������� � json-����� ("models.json")
GraphicObject Scene::createGraphicObject(std::string model)
{
	// �������� ��� ������
	auto modelDescr = document[model.c_str()].GetObjectW();

	string meshFileName = modelDescr["mesh"].GetString();
	string textureFileName = modelDescr["texture"].GetString();
	string materialFileName = modelDescr["material"].GetString();
	string type = modelDescr["type"].GetString();
	vec3 dimensions;
	dimensions.x = modelDescr["dimensions"][0].GetFloat();
	dimensions.y = modelDescr["dimensions"][1].GetFloat();
	dimensions.z = modelDescr["dimensions"][2].GetFloat();

	// ������� ����������� ������
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

	// ���������� ��������� ������
	return grObj;
}

// �����������, ����� �� ������ � ����������� �� �������� �� ����
bool Scene::lodTest(GraphicObject& graphicObject)
{
	float distance = glm::distance(camera->getPosition(), graphicObject.getPosition());

	static const float max_distance[] = { 1200, 1200, 1200, 500, 350.0, 200.0, 400.0, 300.0, 200.0 };

	if (distance < max_distance[static_cast<int>(graphicObject.getType())]) return true;

	return false;
}

// �����������, �������� �� ������ � �������� �������� ���������
bool Scene::frustumCullingTest(GraphicObject& graphicObject)
{
	// �������� ��� ������� ��������������� ������
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

	// ������� ������������� � ��������� ������� ��������� (clip space)
	mat4 PVM = camera->getProjectionMatrix() * camera->getViewMatrix() * graphicObject.getModelMatrix();

	// ��������� ������������ ���� ������ ������������ ���� 6 ���������� frustum'�
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

	// ���������� ���� �� ����� ���� �� ���� �������
	for (int i = 0; i < 6; i++) {
		if (flags[i] >= 8) return false;
	}

	return true;
}