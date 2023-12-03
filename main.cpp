#include <windows.h>
#include <stdio.h>
#include <chrono>
#include <vector>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

#include "ResourceManager.h"
#include "RenderManager.h"
#include "Scene.h"

#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"

// ������������ ������������ ���� (��� ��������)
using namespace glm;
using namespace std;

// ������������ ������
Camera camera;


// ������
Camera frustumCullingCamera;

// ������������ �������� �����
Light light;

// ������������ �����
Scene scene;

// ������� � ��������� ���������� ������ � �������
// ������� ������ ���������� � ������� display ��� ��������� ������� �����
int getFps();

// ����������� ������� ��������� (������� ����� ����� ����������������� �������� ������ �������)
// ������� ���������� � ������� simulation � ���������� ����� � ��������
float getSimulationTime();

// ������� ���������� ��� ����������� ����
// � ��� ����� �������������, �� ������� glutPostRedisplay
void display()
{
	// ��� �������� ���������� ������ �� RenderManager
	RenderManager& renderManager = RenderManager::instance();

	// �������� ����� ������ �����
	renderManager.start();

	// ������� �����
	scene.draw();

	// ��������� ���������� �����
	renderManager.finish();

	// ������ �������� � ������ ������ �����
	glutSwapBuffers();

	// ���������� FPS � ��� ����� � ��������� ����
	char windowTitle[256];
	sprintf_s(windowTitle, 256, "FrustumCulling [%i FPS][%s]", 
		getFps(),
		scene.getSceneDescription().c_str()
	);
	glutSetWindowTitle(windowTitle);
}

// �������, ���������� ��� ��������� �������� ����
void reshape(int w,int h)
{
	// ���������� ����� ������� ���������, ������ ���� ������� ����
	RenderManager::instance().setWindowSize(w, h);
	// ������������� ������� ��������
	camera.setProjectionMatrix(35.0f, (float)w / h, 1.0f, 500.0f);
}

// ������� ��������� - ���������� ����������� �����
// ����� ������� �������������� ���������� �������
void simulation()
{
	// ���������� ������� ���������
	float simulationTime = getSimulationTime();

	// ������������ ������
	float speed = 2.22f * 5;
	float dx = 0.0;
	float dz = 0.0;
	if (GetAsyncKeyState(VK_LEFT) != 0) dx = 1.0;
	if (GetAsyncKeyState(VK_RIGHT) != 0) dx = -1.0;
	dx = dx * simulationTime * speed;
	if (GetAsyncKeyState(VK_UP) != 0) dz = 1.0;
	if (GetAsyncKeyState(VK_DOWN) != 0) dz = -1.0;
	dz = dz * simulationTime * speed;
	camera.moveOXZ(dx, dz);
	//frustumCullingCamera.moveOXZ(dx, dz);

	// �������� ������
	static int prevx = 0;
	static int prevy = 0;
	float speedR = 0.5;
	POINT cur_pos;
	GetCursorPos(&cur_pos);
	if (GetAsyncKeyState(VK_RBUTTON) != 0) {
		float rx = (cur_pos.x - prevx) * speedR;
		float ry = (cur_pos.y - prevy) * speedR;
		//camera.rotate(rx, ry);
		frustumCullingCamera.rotate(rx, ry);
	}
	prevx = cur_pos.x;
	prevy = cur_pos.y;

	// ��������� �������� ������������� ������������ ����
	glutPostRedisplay();
};

// ������� ��������� �������� �������� �����
void mouseWheel(int wheel, int direction, int x, int y)
{
	// ���������� �� ������� ���������� ����������/������� ������
	float zoomSpeed = -0.3f * 5;
	float delta = direction * zoomSpeed;
	camera.zoom(delta);
}

// ��������� ������
void keyboard(unsigned char key, int x, int y)
{
	if (key == '1') {
		cout << "KEY" << endl;
	}
}

// �������� �������
void main(int argc, char **argv)
{
	// ������������� ���������� GLUT
	glutInit(&argc,argv);
	// ������������� ������� (������ ������)
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// ���������� � ������ OpenGL (������ 3.3 ��� ��������� �������� �������������)
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// ������������� ������� ����� ���� ����
	glutInitWindowPosition(300,100);
	// ������������� ������ ����
	glutInitWindowSize(800,600);
	// �������� ����
	glutCreateWindow("Initializing ...");

	// ������������� GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err) {
	  fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
	  return;
	}

	// ����������� ������� ������ OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	// ������������� ���������� DevIL ��� �������� �����������
	ilInit();
	iluInit();
	ilutInit();

	// ������������� ������-���������
	RenderManager::instance().init();
	RenderManager::instance().setWindowSize(800, 600);
	RenderManager::instance().setCamera(&camera);

	// 
	frustumCullingCamera.setProjectionMatrix(35.0f, 800.0 / 600.0, 1.0f, 500.0f);
	RenderManager::instance().setFrustumCullingCamera(&frustumCullingCamera);
	
	// 
	RenderManager::instance().setLight(&light);

	// ������������� �����
	scene.init("data\\models.json");
	scene.loadFromJson("data\\SCENES\\big_scene.json");
	
	//scene.setCamera(&camera);
	scene.setCamera(&frustumCullingCamera);

	scene.setLight(&light);
	

	// ������������� �������, ������� ����� ���������� ��� ����������� ����
	glutDisplayFunc(display);
	// ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
	glutReshapeFunc(reshape);
	// ������������� ������� ������� ���������� ������ ���, ����� ��������� �����������
	glutIdleFunc(simulation);
	// ������������� ������� ������� ���������� ��� ��������� �������� �����
	glutMouseWheelFunc(mouseWheel);
	// ������������� ������� ��� ����������
	glutKeyboardFunc(keyboard);

	// �������� ���� ��������� ��������� ��
	glutMainLoop();	
	return;
}

// ������� � ��������� ���������� ������ � �������
// ������� ������ ���������� � ������� display ��� ��������� ������� �����
int getFps()
{
	// ����� ���������� ���������� FPS (FPS �� ������� �������)
	static int fps = 0;
	// ���������� ������ ������� ������ ������� � ������� ���������� ������ �������
	static int frameCount = 0;
	// ������ ������ �������
	static auto startTime = chrono::steady_clock::now();

	// ����������� ���������� ���������� ������ �� 1 (������� ���������� � ������� display)
	frameCount++;

	// �������� ������� ����� � ���������� ������� ������ ������ � ������� ���������� ������ �������
	auto now = chrono::steady_clock::now();
	chrono::duration<float> delta = now - startTime;

	// ���� ������ ������ 0.5 �������, �� ���������� ���������� FPS � �������� ������ ������
	if (delta >= chrono::milliseconds(500)) {
		fps = static_cast<int>(frameCount / delta.count());
		frameCount = 0;
		startTime = now;
	}

	// ���������� ���������� fps
	return fps;
}

// ����������� ������� ��������� (������� ����� ����� ����������������� �������� ������ �������)
// ������� ���������� � ������� simulation � ���������� ����� � ��������
float getSimulationTime()
{
	// ����� �� ���������� ������ �������
	static auto previousTime = chrono::steady_clock::now();

	// �������� ������� ����� � ���������� ������� ������ ������ � ����������� ������
	auto now = chrono::steady_clock::now();
	chrono::duration<float> delta = now - previousTime;

	// ���������� ����� ������ ��������� ��������
	previousTime = now;

	// ���������� ��������� ����� � �������� (������)
	return delta.count();
}
