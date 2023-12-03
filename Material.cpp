#include "Material.h"

#include "rapidjson/document.h"

using namespace std;
using namespace glm;
using namespace rapidjson;

//	����������� �� ���������
Material::Material()
{
	diffuse = vec4(0.85, 0.85, 0.85, 1);
	ambient = vec4(0.35, 0.35, 0.35, 1);
	specular = vec4(0.25, 0.25, 0.25, 1);	
	shininess = 64.0;
};

// �������� ���������� ��������� �� json-�����
bool Material::loadFromJson(string filename)
{
	cout << "Loading material from " << filename << endl;

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


	// ������ - ���� ������, �� �������
	Document document;
	document.Parse(jsonString.c_str());
	if (document.GetParseError() != 0) {
		cout << "�������� ������ ����� " << filename << endl;
		return false;
	}

	// ������
	ambient.x = document["ambient"][0].GetFloat();
	ambient.y = document["ambient"][1].GetFloat();
	ambient.z = document["ambient"][2].GetFloat();
	diffuse.x = document["diffuse"][0].GetFloat();
	diffuse.y = document["diffuse"][1].GetFloat();
	diffuse.z = document["diffuse"][2].GetFloat();
	specular.x = document["specular"][0].GetFloat();
	specular.y = document["specular"][1].GetFloat();
	specular.z = document["specular"][2].GetFloat();
	shininess = document["shininess"].GetFloat();

	// �������� ���������� ������
	return true;
}

vec4& Material::getAmbient()
{
	return ambient;
}
vec4& Material::getDiffuse()
{
	return diffuse;
}
vec4& Material::getSpecular()
{
	return specular;
}
float Material::getShininess()
{
	return shininess;
}
