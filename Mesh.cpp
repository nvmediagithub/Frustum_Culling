#include "Mesh.h"

using namespace glm;
using namespace std;

// Структура, описывающая одну вершину полигональной сетки 
// каждая вершина имеет геометрические координаты, 
// вектор нормали и текстурные координаты
struct Vertex
{
	// геометрические координаты
	GLfloat coord[3];
	// вектор нормали
	GLfloat normal[3];
	// текстурные координаты нулевого текстурного блока	
	GLfloat texCoord[2];
};


// конструктор
Mesh::Mesh()
{
	vao = 0;
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexCount = 0;
}

// загрузка меша из внешнего obj-файла
bool Mesh::load(std::string filename)
{
	// ЗАГРУЗКА ДАННЫХ ИЗ ФАЙЛА:
	//cout << endl << "loading file " << filename << endl;

	// пробуем открыть файл (const char* filename - имя открываемого файла)
	ifstream f;
	f.open(filename);
	if (!f.is_open()) {
		cout << "Can not open file " << filename << endl;
		return false;
	};

	// строка для чтения первого слова
	string s;
	// временая строка для чтения "мусора" - оставшейся части строки
	char temp[512];
	// вектор для хранения геометрических координат
	vector<vec3> v;
	// вектор для хранения нормалей
	vector<vec3> n;
	// вектор для хранения текстурных координат
	vector<vec2> t;
	// вектор для хранения индексов атрибутов, для построения вершин
	vector<ivec3> faces;
	// чтение первых слов каждой строки (пока не конец файла)
	while (!f.eof()) {
		// считываем первое слово в строке
		f >> s;
		// считываем геометрические координаты:
		if (s == "v") {
			vec3 vTemp;
			f >> vTemp.x >> vTemp.y >> vTemp.z;
			v.push_back(vTemp);
		}
		// считываем нормали:
		if (s == "vn") {
			vec3 vTemp;
			f >> vTemp.x >> vTemp.y >> vTemp.z;
			n.push_back(vTemp);
		}
		// считываем текстурные координаты
		if (s == "vt") {
			vec2 vTemp;
			f >> vTemp.x >> vTemp.y;
			t.push_back(vTemp);
		}
		// чтение информации об одном полигоне
		if (s == "f") {
			ivec3 face;
			char ch;
			f >> face.x >> ch >> face.y >> ch >> face.z;
			faces.push_back(face);
			f >> face.x >> ch >> face.y >> ch >> face.z;
			faces.push_back(face);
			f >> face.x >> ch >> face.y >> ch >> face.z;
			faces.push_back(face);
		}
		// считываем оставшуюся часть строки
		// исходим из предположения, 
		// что она не может быть больше 512 символов
		f.getline(temp, 512);
	};

	// закрываем файл
	f.close();

	// ОБРАБОТКА ДАННЫХ И ФОРМИРОВАНИЕ МАССИВОВ ДЛЯ OPENGL:
	// массив вершин
	vector<Vertex> vertices;
	// массив индексов
	vector<GLuint> indices;
	// отображение вершины (по используемым ею атрибутам) на индекс в массиве вершин
	map<string, int> vertexToIndex;

	// создаем массив вершин
	for (int i = 0; i < faces.size(); i++) {

		ivec3 vi = faces[i];
		string key = to_string(vi.x) + "/" + to_string(vi.y) + "/" + to_string(vi.z);
		auto it = vertexToIndex.find(key);
		if (it != vertexToIndex.end()) {
			// существующая вершина
			indices.push_back(it->second);
		}
		else {
			// новая вершина:
			// добавляем в массив вершин
			Vertex vtx;
			vtx.coord[0] = v[vi.x - 1].x;
			vtx.coord[1] = v[vi.x - 1].y;
			vtx.coord[2] = v[vi.x - 1].z;
			vtx.texCoord[0] = t[vi.y - 1].s;
			vtx.texCoord[1] = t[vi.y - 1].t;
			vtx.normal[0] = n[vi.z - 1].x;
			vtx.normal[1] = n[vi.z - 1].y;
			vtx.normal[2] = n[vi.z - 1].z;
			vertices.push_back(vtx);
			// добавляем массив индексов
			int i = vertices.size() - 1;
			indices.push_back(i);
			// создаем ассоциацию
			vertexToIndex[key] = i;
		};
	}

	// ЗАПОЛНЕНИЕ БУФЕРОВ VBO
	// создание и привязка VAO-объекта
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// создаем и заполняем буфер вершин
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// создаем и заполняем буфер индексов
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	// определяем количество вершин (индексов)
	vertexCount = indices.size();
	// заполнение VAO
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coord));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	// "отвязка" буфера VAO, чтоб случайно не испортить
    glBindVertexArray(0);

    // успешное завершение программы
    return true;
}

// вывод модели
// используется текущий шейдер и ранее установленные значения uniform-переменных
void Mesh::drawOne()
{
	if (vertexCount == 0) return;
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// вывод инстанцированного меша (вывести count мешей) 
void Mesh::drawMany(int count)
{
	if (vertexCount == 0) return;
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0, count);
	glBindVertexArray(0);
}
