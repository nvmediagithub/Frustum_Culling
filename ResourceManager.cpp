#include "ResourceManager.h"

using namespace glm;
using namespace std;

// �������� ������ ����.
// ������������ ��������: ������ ���� � ��������� ��������.
int ResourceManager::loadMesh(std::string filename)
{
	int index = -1;
	auto it = meshes_id.find(filename);
	if (it == meshes_id.end()) {
		index = meshes.size();
		meshes.emplace_back();
		meshes[index].load(filename);
		meshes_id.insert(pair<string, int>(filename, index));
	}
	else {
		index = it->second;
	}
	return index;
}

// ��������� ���� �� ��� �������.
// ���� ������ ���� ��� (�� �������������� ������) ������������ nullptr
Mesh* ResourceManager::getMesh(int index)
{
	if (index < meshes.size() && index >= 0) {
		return &meshes[index];
	}
	return nullptr;
}

// �������� ����� ��������.
// ������������ �������� - ������ �������� � ��������� ��������.
int ResourceManager::loadTexture(std::string filename)
{
	int index = -1;
	auto it = textures_id.find(filename);
	if (it == textures_id.end()) {
		index = textures.size();
		textures.emplace_back();
		textures[index].load(filename);
		textures_id.insert(pair<string, int>(filename, index));
	}
	else {
		index = it->second;
	}
	return index;
}

// ��������� �������� �� � �������.
// ���� ����� �������� ��� (���������������� ������) ������������ nullptr
Texture* ResourceManager::getTexture(int index)
{
	if (index < textures.size() && index >= 0) {
		return &textures[index];
	}
	return nullptr;
}

// �������� ������ ���������
// ������������ �������� - ������ ��������� � ��������� ��������.
int ResourceManager::loadMaterial(std::string filename)
{
	int index = -1;
	auto it = materials_id.find(filename);
	if (it == materials_id.end()) {
		index = materials.size();
		materials.emplace_back();
		materials[index].loadFromJson(filename);
		materials_id.insert(pair<string, int>(filename, index));
	}
	else {
		index = it->second;
	}
	return index;

}

// ��������� �������� �� � �������.
// ���� ����� �������� ��� (���������������� ������) ������������ nullptr
Material* ResourceManager::getMaterial(int index)
{
	if (index < materials.size() && index >= 0) {
		return &materials[index];
	}
	return nullptr;
}