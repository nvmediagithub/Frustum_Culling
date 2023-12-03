#include <algorithm>

#include "RenderManager.h"

using namespace glm;
using namespace std;


// ������������� ������� RenderManager, ����������� ����� ������������� OpenGL:
// �������� �������� � �.�.
void RenderManager::init()
{
	// �������� �������
	shaders.emplace_back();
	shaders[0].load("DATA\\SHADERS\\DiffuseTexture.vsh", "DATA\\SHADERS\\DiffuseTexture.fsh");

	shaders.emplace_back();
	shaders[1].load("DATA\\SHADERS\\ViewingFrustum.vsh", "DATA\\SHADERS\\ViewingFrustum.fsh");


	// ���������� ����� ������������� �������� � ��������
	previouseTextureID = -1;
	previouseMaterialID = -1;
}


// ������ ������ ���������� �����
void RenderManager::start()
{
	// ������� ��� ����� ������������� �������� ������� GraphicObjects
	graphicObjects.clear();
	// ���������� ����� ������������� �������� � ��������
	previouseTextureID = -1;
	previouseMaterialID = -1;
}

// ��������� ������������ ������
void RenderManager::setCamera(Camera* �amera)
{
	this->camera = �amera;
}

void RenderManager::setFrustumCullingCamera(Camera* frustumCullingCamera)
{
	this->frustumCullingCamera = frustumCullingCamera;
}

// ��������� ������������� ��������� �����
void RenderManager::setLight(Light *light)
{
	this->light = light;
}

// ���������� � ������� ���������� ���������� ������� ��� ������
void RenderManager::addToRenderQueue(GraphicObject& graphicObject)
{
	graphicObjects.push_back(graphicObject);
	instancedMvMatrices.clear();
}

// ���������� ������ �����
void RenderManager::finish()
{
	// �������� ����� �����
	glClearColor(0.880, 0.890, 0.910, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ������������� ��������� �������������� ��������� ����������
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// ������� ��� �������
	drawAllObjects();

	// ����� �������� ���������
	shaders[1].activate();
	mat4 projectionMatrix = camera->getProjectionMatrix();
	mat4 viewMatrix = camera->getViewMatrix();
	shaders[1].setUniform("projectionMatrix", projectionMatrix);
	mat4 modelMatrix = mat4(1.0f);
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	shaders[1].setUniform("modelViewMatrix", modelViewMatrix);
	drawViewFrustum();
}

// ��������� ������� ����
void RenderManager::setWindowSize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}


// ��������� (��������) �������� �� � ID � ��������� ��������
void RenderManager::setTexture(Shader& shader, int textureId)
{
	if (previouseTextureID == textureId) return;
	previouseTextureID = textureId;

	Texture *texture = ResourceManager::instance().getTexture(textureId);
	if (texture != nullptr) texture->bind();
	shader.setUniform("texture_0", 0);
}

// ��������� ���������� ��������� �� ��� ID � ��������� ��������
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


// ����� ���� ����������� ��������
void RenderManager::drawAllObjects()
{
	// ��������������� ���������� �� ����������
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

	// ���������� ������, ������������ ��� ������ �������
	Shader& shader = shaders[0];
	shader = shaders[0];
	shader.activate();

	// ������������� ������� ��������
	mat4& projectionMatrix = camera->getProjectionMatrix();
	shader.setUniform("projectionMatrix", projectionMatrix);

	// �������� ������� ������
	mat4& viewMatrix = camera->getViewMatrix();

	// ������������� ��������� ��������� �����
	// ��� �������� ������ ��� ���� ��������
	shader.setUniform("lAmbient", light->getAmbient());
	shader.setUniform("lDiffuse", light->getDiffuse());
	shader.setUniform("lSpecular", light->getSpecular());
	// ��� ��������� ����������� �� �������� �����
	// ��� ���������� ������������� � ������� ��������� �����������
	shader.setUniform("lPosition", viewMatrix * light->getDirection());

	// ���������� ����������� ������� ���������� ������
	instancedMvMatrices.clear();

	// ������� ��� �������
	for (int i = 0; i < graphicObjects.size(); i++) {

		// ������������� ��������
		int textureId = graphicObjects[i].getTextureId();
		setTexture(shader, textureId);

		// ������������� ��������� ���������
		int materialId = graphicObjects[i].getMaterialId();
		setMaterial(shader, materialId);

		// ����������� ������� ���������� ������
		mat4 modelViewMatrix = viewMatrix * graphicObjects[i].getModelMatrix();
		instancedMvMatrices.push_back(modelViewMatrix);

		// ���� ���������� ������� ���������������� ������, �� ������� ��

		int count = instancedMvMatrices.size();
		// ������������� ��� �������			
		shader.setUniform("modelViewMatrix", modelViewMatrix);

		// ������� ���������������� ���
		int meshId = graphicObjects[i].getMeshId();
		Mesh *mesh = ResourceManager::instance().getMesh(meshId);
		if (mesh != nullptr) {
			mesh->drawMany(count);
		};
		// ��������� ����� ����������������� ����
		instancedMvMatrices.clear();

	}
}


// ������� ������ ��������� �������� ���������
void RenderManager::drawViewFrustum()
{
	mat4& projectionMatrix = frustumCullingCamera->getProjectionMatrix();
	mat4& viewMatrix = frustumCullingCamera->getViewMatrix();

	glm::vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	glm::vec3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;

	// ��������������� �������������� ���������� ��� ����� ������� � ������� ����������
	glm::vec4 corners[8] = {
		glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), // ����� ������ �������� ����
		glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),  // ������ ������ �������� ����
		glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),  // ����� ������� �������� ����
		glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),   // ������ ������� �������� ����
		glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),  // ����� ������ ������ ����
		glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),   // ������ ������ ������ ����
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),   // ����� ������� ������ ����
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)     // ������ ������� ������ ����
	};

	// �������� ���� �� �������� ������� ��������-����
	glm::mat4 inverseVP = glm::inverse(projectionMatrix * viewMatrix);

	for (int i = 0; i < 8; ++i) {
		glm::vec4 worldCoords = inverseVP * corners[i];

		// ����� �� W, ����� �������� ��������������� ������� ����������
		worldCoords /= worldCoords.w;

		// ��������� ���������� � �������� ��������� 
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

	// ���������� �� ������ �������
	static bool init = true;
	static GLuint vao = 0;				// ������ VAO-������
	static GLuint vertexBuffer = 0;		// ������ VBO-������
	static int vertexCount = 0;			// ���������� ������

	// ��� ������ ������ �������������� VBO � VAO
	if (init) {
		init = false;


		// ���������� ������� VBO
		// �������� � �������� VAO-�������
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		// ������� � ��������� ����� ������
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STREAM_DRAW);

		// ���������� ���������� ������ (��������)
		vertexCount = 8;
		// ���������� VAO
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// "�������" ������ VAO, ���� �������� �� ���������
		glBindVertexArray(0);
	}

	// �������
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STREAM_DRAW);
	glDrawArrays(GL_LINES, 0, vertexCount);
	glBindVertexArray(0);
	return;
}