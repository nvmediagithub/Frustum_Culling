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

// ����� ��� ������ �� ������
class Scene
{
public:
	// ����������� �� ���������
	Scene();

	// ������������� �����
	// ��������: json-���� � ��������� ������� (models.json)
	void init(std::string filename);

	// �������� ����� �� json-�����
	bool loadFromJson(std::string filename);

	// ��������� ������������ ������ (������ �� ���������� ������)
	void setCamera(Camera *camera);
	// ��������� ������������� ��������� ����� (������ �� ���������� ������)
	void setLight(Light *light);

	// ����� ���� ����� (����������� ��������� � RenderManager)
	void draw();

	// ��������� ���������� � ����� ��� �������:
	// [���������� �������� ���������� �� ���������] / [����� ���������� �������� � �����]
	std::string getSceneDescription();

private:
	// �������� ������� �� ��� �������������� � json-����� ("models.json")
	GraphicObject createGraphicObject(std::string model);
	
	// �����������, ����� �� ������ � ����������� �� �������� �� ����
	bool lodTest(GraphicObject& graphicObject);

	// �����������, �������� �� ������ � �������� �������� ���������
	bool frustumCullingTest(GraphicObject& graphicObject);

private:
	// json-������������� ���� ������� (�� ����� "models.json")
	rapidjson::Document document;
	// ��������� �� ������ (�� ���������� ����������)
	Camera *camera;
	// ��������� �� �������� ��������� (�� ���������� ����������)
	Camera *frustumCullingCamera;
	// ��������� �� �������� ����� (�� ���������� ����������)
	Light *light;
	// ��� ����������� �������
	std::vector<GraphicObject> graphicObjects;
	// ���������� - ������� �������� ��������� �� ��������� � ������� �����
	int renderedObjectCount;
};
