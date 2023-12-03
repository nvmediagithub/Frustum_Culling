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

// ����� ��� �������������� � OPENGL
// ���� ����� �������������� ����� ������������ ��������� ������� ������
class RenderManager
{
private:
	// ����������� �� ��������� (���������)
	RenderManager() {};
	// ������������ ����������� ���
	RenderManager(const RenderManager& root) = delete;
	// ��������� ������������ ���
	RenderManager& operator=(const RenderManager&) = delete;

public:
	// ������-����� ��� ��������� ���������� ��������� �������.
	static RenderManager& instance()
	{
		static RenderManager renderManager;
		return renderManager;
	}

	// ������������� ������� RenderManager, ����������� ����� ������������� OpenGL:
	// �������� ��������, ��������� ���������� ���������� � ������ �������������
	void init();

	// ��������� ������� ����
	void setWindowSize(int w, int h);

	// ������ ������ ���������� ����� (����������, ������� ������� ����������� ��������)
	void start();

	// ��������� ������������ ������
	void setCamera(Camera *camera);

	//
	void setFrustumCullingCamera(Camera* frustumCullingCamera);

	// ��������� ������������� ��������� �����
	void setLight(Light *light);
	// ���������� � ������� ���������� ���������� ������� ��� ������
	void addToRenderQueue(GraphicObject& graphicObject);

	// ���������� ������ ����� (�������� ������)
	void finish();


private:
	// ��������� (��������) �������� �� � ID � ��������� ��������
	void setTexture(Shader& shader, int textureId);
	// ��������� ���������� ��������� �� ��� ID � ��������� ��������
	void setMaterial(Shader& shader, int materialId);

	// ����� ���� ����������� ��������
	void drawAllObjects();
	// ����� �������� ���������
	void drawViewFrustum();

private:
	// ������������ ������� 
	std::vector<Shader>	shaders;

	// ������� ����
	int windowWidth, windowHeight;

	// ��������� �� ������
	Camera* camera;

	// ��������� �� ������ ��������� ��������
	Camera* frustumCullingCamera;
	
	// ��������� �� �������� �����
	Light* light;

	// ����� ������������� ��������
	int previouseTextureID;
	// ����� ������������� ��������
	int previouseMaterialID;

	// ����� ����-���������
	int postProcessingMode;

	// ������ ����������� ��������, ������� ���������� ������� � ������ �����
	std::vector<GraphicObject> graphicObjects;

	// ������ ��� ���������� ������ ������� ��� �����������
	std::vector<glm::mat4> instancedMvMatrices;
};
