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

// ����� ��� ������ � ���������� �������� (���������� �� ������ ������� SINGLTON)
class ResourceManager 
{
public:
	// ������-����� ��� ��������� ���������� ��������� �������.
	// ������ ����� ���������� ������ �� ������, ���������� � ������������ ����������.
	static ResourceManager& instance()
	{
		static ResourceManager ResourceManager;
		return ResourceManager;
	}

    // �������� ������ ���� �� ���������� obj-�����
    // ������������ �������� - ������ ���� � ��������� ��������
	int loadMesh(std::string filename);

    // ��������� ���� �� ��� �������
    // ���� ������ ���� ��� (���������������� ������) ������������ nullptr
	Mesh* getMesh(int index);

	// �������� ����� ��������.
	// ������������ �������� - ������ �������� � ��������� ��������.
	int loadTexture(std::string filename);

	// ��������� �������� �� � �������.
	// ���� ����� �������� ��� (���������������� ������) ������������ nullptr
	Texture* getTexture(int index);

	// �������� ������ ���������
	// ������������ �������� - ������ ��������� � ��������� ��������.
	int loadMaterial(std::string filename);

	// ��������� �������� �� � �������.
	// ���� ����� �������� ��� (���������������� ������) ������������ nullptr
	Material* getMaterial(int index);

private:
	// ����������� �� ��������� (�������� ���������)
	// � ���������� ������ ������� �� ������ ������� ������� ������ ��� ������ ������
	ResourceManager() {};
	// ������������ ����������� ��� (������)
	ResourceManager(const ResourceManager& v) = delete;
	// ��������� ������������ ��� (������)
	ResourceManager& operator=(const ResourceManager& v) = delete;

private:
	// ������ ��� �������� ���� �����
	std::vector<Mesh> meshes;
	// map ��� �������� ������������ ����� ������ �������������� ����� 
	// � �������� � ���������� meshes 
	std::map <std::string, int> meshes_id;

	// ������ ��� �������� ���� �������
	std::vector<Texture> textures;
	// map ��� �������� ������������ ����� ������ �������������� ����� 
	// � �������� � ���������� textures 
	std::map <std::string, int> textures_id;

	// ������ ��� �������� ���� ����������
	std::vector<Material> materials;
	// map ��� �������� ������������ ����� ������ �������������� ����� 
	// � �������� � ���������� materials 
	std::map <std::string, int> materials_id;
};
