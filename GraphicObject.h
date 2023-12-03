#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ��� ������ ������������ �������
enum class GraphicObjectType { none, road, building, vehicle, 
	big_nature, small_nature, big_prop, medium_prop, small_prop };


// ����� ��� ������ � ����������� ��������
class GraphicObject
{
public:
	// ����������� �� ���������
	GraphicObject();

	// ���������� ��������� ���������
	void setColor(glm::vec4 &color);
	void setPosition(glm::vec3 &position);
	void setAngle(float degree);
	void setType(GraphicObjectType type);
	void setDimensions(glm::vec3 &dimensions);

	// ���������� �������������� ��������
	void setMeshId(int id);
	void setTextureId(int id);
	void setMaterialId(int id);

	// �������� ��������� ��������� 
	glm::vec4 getColor();
	glm::vec3 getPosition();
	float getAngle();
	GraphicObjectType getType();
	glm::vec3 getDimensions();

	// �������� �������������� ��������
	int getMeshId();
	int getTextureId();
	int getMaterialId();

	// �������� ������� ������
	glm::mat4& getModelMatrix();

private:
	// ���� �������
	glm::vec4 color;
	// ��� ������
	GraphicObjectType type;
	// ������ ������ (W x H x D)
	glm::vec3 dimensions;
	// �������
	glm::vec3 position;
	// ���� �������� � �������������� ��������� � ��������, �� ������� �������
	float angle;

	// ������� ������ (������ ������� � ����������)
	glm::mat4 modelMatrix;

	// ������������� ������������� ����
	int meshId;
	// ������������� ������������ ��������
	int textureId;
	// ������������� ������������� ���������
	int materialId;
};
