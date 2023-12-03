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

// используемые пространства имен (для удобства)
using namespace glm;
using namespace std;

// ИСПОЛЬЗУЕМАЯ КАМЕРЫ
Camera camera;


// КАМЕРА
Camera frustumCullingCamera;

// ИСПОЛЬЗУЕМЫЙ ИСТОЧНИК СВЕТА
Light light;

// ИСПОЛЬЗУЕМАЯ СЦЕНА
Scene scene;

// подсчет и получение количества кадров в секунду
// функция должна вызываться в функции display при отрисовке каждого кадра
int getFps();

// определение времени симуляции (времени между двумя последовательными вызовами данной функции)
// функция вызывается в функции simulation и возвращает время в секундах
float getSimulationTime();

// функция вызывается при перерисовке окна
// в том числе принудительно, по команде glutPostRedisplay
void display()
{
	// для удобства определяем ссылку на RenderManager
	RenderManager& renderManager = RenderManager::instance();

	// начинаем вывод нового кадра
	renderManager.start();

	// выводим сцену
	scene.draw();

	// завершаем построение кадра
	renderManager.finish();

	// меняем передний и задний буферы цвета
	glutSwapBuffers();

	// вычисление FPS и его вывод в заголовок окна
	char windowTitle[256];
	sprintf_s(windowTitle, 256, "FrustumCulling [%i FPS][%s]", 
		getFps(),
		scene.getSceneDescription().c_str()
	);
	glutSetWindowTitle(windowTitle);
}

// функция, вызываемая при изменении размеров окна
void reshape(int w,int h)
{
	// установить новую область просмотра, равную всей области окна
	RenderManager::instance().setWindowSize(w, h);
	// устанавливаем матрицу проекции
	camera.setProjectionMatrix(35.0f, (float)w / h, 1.0f, 500.0f);
}

// функция симуляции - вызывается максимально часто
// через заранее неопределенные промежутки времени
void simulation()
{
	// получениче времени симуляции
	float simulationTime = getSimulationTime();

	// передвижение камеры
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

	// вращение камеры
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

	// установка признака необходимости перерисовать окно
	glutPostRedisplay();
};

// функция обработки вращения колесика мышки
void mouseWheel(int wheel, int direction, int x, int y)
{
	// определяем на сколько необходимо приблизить/удалить камеру
	float zoomSpeed = -0.3f * 5;
	float delta = direction * zoomSpeed;
	camera.zoom(delta);
}

// обработка клавиш
void keyboard(unsigned char key, int x, int y)
{
	if (key == '1') {
		cout << "KEY" << endl;
	}
}

// основная функция
void main(int argc, char **argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc,argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300,100);
	// устанавливаем размер окна
	glutInitWindowSize(800,600);
	// создание окна
	glutCreateWindow("Initializing ...");

	// инициализация GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err) {
	  fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
	  return;
	}

	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	// инициализация библиотеки DevIL для загрузки изображений
	ilInit();
	iluInit();
	ilutInit();

	// инициализация рендер-менеджера
	RenderManager::instance().init();
	RenderManager::instance().setWindowSize(800, 600);
	RenderManager::instance().setCamera(&camera);

	// 
	frustumCullingCamera.setProjectionMatrix(35.0f, 800.0 / 600.0, 1.0f, 500.0f);
	RenderManager::instance().setFrustumCullingCamera(&frustumCullingCamera);
	
	// 
	RenderManager::instance().setLight(&light);

	// инициализация сцены
	scene.init("data\\models.json");
	scene.loadFromJson("data\\SCENES\\big_scene.json");
	
	//scene.setCamera(&camera);
	scene.setCamera(&frustumCullingCamera);

	scene.setLight(&light);
	

	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(reshape);
	// устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(simulation);
	// устанавливаем функцию которая вызывается при прокрутке колесика мышки
	glutMouseWheelFunc(mouseWheel);
	// устанавливаем функцию для клавиатуры
	glutKeyboardFunc(keyboard);

	// основной цикл обработки сообщений ОС
	glutMainLoop();	
	return;
}

// подсчет и получение количества кадров в секунду
// функция должна вызываться в функции display при отрисовке каждого кадра
int getFps()
{
	// ранее замеренное количество FPS (FPS за прошлую секунду)
	static int fps = 0;
	// количество кадров которые успели вывести с момента последнего начала отсчета
	static int frameCount = 0;
	// момент начала отсчета
	static auto startTime = chrono::steady_clock::now();

	// увеличиваем количество выведенных кадров на 1 (функция вызывается в функции display)
	frameCount++;

	// получаем текущее время и определяем сколько прошло времни с момента последнего начала отсчета
	auto now = chrono::steady_clock::now();
	chrono::duration<float> delta = now - startTime;

	// если прошло больше 0.5 секунды, то запоминаем количество FPS и начинаем отсчет заново
	if (delta >= chrono::milliseconds(500)) {
		fps = static_cast<int>(frameCount / delta.count());
		frameCount = 0;
		startTime = now;
	}

	// возврашаем количество fps
	return fps;
}

// определение времени симуляции (времени между двумя последовательными вызовами данной функции)
// функция вызывается в функции simulation и возвращает время в секундах
float getSimulationTime()
{
	// время на предыдущем вызове функции
	static auto previousTime = chrono::steady_clock::now();

	// получаем текущее время и определяем сколько прошло времни с предыдущего вызова
	auto now = chrono::steady_clock::now();
	chrono::duration<float> delta = now - previousTime;

	// предыдущее время теперь равняется текущему
	previousTime = now;

	// возврашаем прошедшее время в секундах (дробно)
	return delta.count();
}
