#include "Shader.h"

using namespace glm;
using namespace std;

// id "текущей" выбранной шейдерной программы
GLuint Shader::currentProgram = 0;

// загрузить шейдер
bool Shader::load(string veftexShaderFilename, string fragmentShaderFilename)
{
	// создание вершинного шейдерного объекта
	GLuint vsh = createShaderObject(GL_VERTEX_SHADER, veftexShaderFilename);
	if (vsh == -1) return false;
	// создание фрагментного шейдерного объекта
	GLuint fsh = createShaderObject(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	if (fsh == -1) return false;

	// создание шейдерной программы и линковка
	program = glCreateProgram();
	glAttachShader(program, vsh);
	glAttachShader(program, fsh);
	glLinkProgram(program);

	// проверка результата линковки и вывод сообщения об ошибке, если необходимо
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		GLuint count;
		char errorLog[1024];
		glGetProgramInfoLog(program, 1024, (GLsizei*)&count, errorLog);

		cout << "Shader program link error for files \"" << veftexShaderFilename << "\" and \"" << fragmentShaderFilename << "\":" << endl;
		cout << errorLog << endl;

		return false;
	}

	cout << "Shader program for files \"" << veftexShaderFilename << "\" and \"" << fragmentShaderFilename << "\" has been created with id=" << program << endl;
	return true;
}

// создание шейдерного объекта указанного типа и загрузка исходного текста шейдера из указанного файла
GLuint Shader::createShaderObject(GLenum shaderType, std::string filename)
{
	// буфер для использования
	const int max_buffer_size = 1024 * 10;
	char buffer[max_buffer_size];

	// открываем файл
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "File \"" << filename << "\" can not be found" << endl;
		return -1;
	}

	// считываем содержимое файла
	file.getline(buffer, max_buffer_size, 0);

	// создание шейдера
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) {
		cout << "Shader object can't be created" << endl;
		return -1;
	}

	// указание текста шейдера
	const char* source_pointer[] = { buffer };
	glShaderSource(shader, 1, source_pointer, 0);
	//cout << buffer;

	// компилирование шейдеров
	glCompileShader(shader);

	// проверка результата компилирования и вывод сообщения об ошибке, если необходимо
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) {
		GLuint count;
		glGetShaderInfoLog(shader, max_buffer_size, (GLsizei*)&count, buffer);

		cout << "Shader object for file \"" << filename << "\" was compiled with error:" << endl;
		cout << buffer << endl;

		return -1;
	}

	// если все нормально, возвращаем идентификатор шейдера
	return shader;
}

// активизировать шейдер (сделать текущим)
void Shader::activate ()
{
	if (currentProgram != program) {
		currentProgram = program;
		glUseProgram(program);
	}
}

// отключить шейдер (использовать нулевую шейдерную программу)
void Shader :: deactivate ()
{
	if (currentProgram != 0) {
		currentProgram = 0;
		glUseProgram(0);
	}
}

// получение индекса (location) uniform-переменной
GLuint Shader::getUniformLocation(std::string name)
{
	int location = -1;
	auto it = uniforms.find(name);
	if (it == uniforms.end()) {
		location = glGetUniformLocation(program, name.c_str());
		uniforms[name] = location;
	}
	else {
		location = it->second;
	};
	return location;
}

void Shader::setUniform(std::string name, int value)
{
	// выбираем шейдер (если в этом есть необходимость)
	activate();
	// устанавливаем значение uniform-переменной
	auto location = getUniformLocation(name);
	if (location < 0) return;
	glUniform1i(location, value);
}

void Shader::setUniform(std::string name, float value)
{
	// выбираем шейдер (если в этом есть необходимость)
	activate();
	// устанавливаем значение uniform-переменной
	auto location = getUniformLocation(name);
	if (location < 0) return;
	glUniform1f(location, value);
}

void Shader::setUniform(string name, vec2 &value)
{
	// выбираем шейдер (если в этом есть необходимость)
	activate();
	// устанавливаем значение uniform-переменной
	auto location = getUniformLocation(name);
	if (location < 0) return;
	glUniform2fv(location, 1, value_ptr(value));
}

void Shader::setUniform(string name, vec4 &value)
{
	// выбираем шейдер (если в этом есть необходимость)
	activate();
	// устанавливаем значение uniform-переменной
	auto location = getUniformLocation(name);
	if (location < 0) return;
	glUniform4fv(location, 1, value_ptr(value));
}

void Shader::setUniform(string name, mat4 &value)
{
	// выбираем шейдер (если в этом есть необходимость)
	activate();
	// устанавливаем значение uniform-переменной
	auto location = getUniformLocation(name);
	if (location < 0) return;
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void Shader::setUniform(std::string name, std::vector<glm::mat4> &value)
{
	// выбираем шейдер (если в этом есть необходимость)
	activate();
	
	auto location = getUniformLocation(name);
	if (location < 0) return;

	static GLfloat m[16 * 64];
	int index = 0;
	for (int count = 0; count < value.size(); count++) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[index++] = value[count][i][j];
			}
		}
	};

	glUniformMatrix4fv(location, value.size(), GL_FALSE, m);
}
