#include "Mesh.h"

using namespace glm;
using namespace std;

// ���������, ����������� ���� ������� ������������� ����� 
// ������ ������� ����� �������������� ����������, 
// ������ ������� � ���������� ����������
struct Vertex
{
	// �������������� ����������
	GLfloat coord[3];
	// ������ �������
	GLfloat normal[3];
	// ���������� ���������� �������� ����������� �����	
	GLfloat texCoord[2];
};


// �����������
Mesh::Mesh()
{
	vao = 0;
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexCount = 0;
}

// �������� ���� �� �������� obj-�����
bool Mesh::load(std::string filename)
{
	// �������� ������ �� �����:
	//cout << endl << "loading file " << filename << endl;

	// ������� ������� ���� (const char* filename - ��� ������������ �����)
	ifstream f;
	f.open(filename);
	if (!f.is_open()) {
		cout << "Can not open file " << filename << endl;
		return false;
	};

	// ������ ��� ������ ������� �����
	string s;
	// �������� ������ ��� ������ "������" - ���������� ����� ������
	char temp[512];
	// ������ ��� �������� �������������� ���������
	vector<vec3> v;
	// ������ ��� �������� ��������
	vector<vec3> n;
	// ������ ��� �������� ���������� ���������
	vector<vec2> t;
	// ������ ��� �������� �������� ���������, ��� ���������� ������
	vector<ivec3> faces;
	// ������ ������ ���� ������ ������ (���� �� ����� �����)
	while (!f.eof()) {
		// ��������� ������ ����� � ������
		f >> s;
		// ��������� �������������� ����������:
		if (s == "v") {
			vec3 vTemp;
			f >> vTemp.x >> vTemp.y >> vTemp.z;
			v.push_back(vTemp);
		}
		// ��������� �������:
		if (s == "vn") {
			vec3 vTemp;
			f >> vTemp.x >> vTemp.y >> vTemp.z;
			n.push_back(vTemp);
		}
		// ��������� ���������� ����������
		if (s == "vt") {
			vec2 vTemp;
			f >> vTemp.x >> vTemp.y;
			t.push_back(vTemp);
		}
		// ������ ���������� �� ����� ��������
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
		// ��������� ���������� ����� ������
		// ������� �� �������������, 
		// ��� ��� �� ����� ���� ������ 512 ��������
		f.getline(temp, 512);
	};

	// ��������� ����
	f.close();

	// ��������� ������ � ������������ �������� ��� OPENGL:
	// ������ ������
	vector<Vertex> vertices;
	// ������ ��������
	vector<GLuint> indices;
	// ����������� ������� (�� ������������ �� ���������) �� ������ � ������� ������
	map<string, int> vertexToIndex;

	// ������� ������ ������
	for (int i = 0; i < faces.size(); i++) {

		ivec3 vi = faces[i];
		string key = to_string(vi.x) + "/" + to_string(vi.y) + "/" + to_string(vi.z);
		auto it = vertexToIndex.find(key);
		if (it != vertexToIndex.end()) {
			// ������������ �������
			indices.push_back(it->second);
		}
		else {
			// ����� �������:
			// ��������� � ������ ������
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
			// ��������� ������ ��������
			int i = vertices.size() - 1;
			indices.push_back(i);
			// ������� ����������
			vertexToIndex[key] = i;
		};
	}

	// ���������� ������� VBO
	// �������� � �������� VAO-�������
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// ������� � ��������� ����� ������
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// ������� � ��������� ����� ��������
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	// ���������� ���������� ������ (��������)
	vertexCount = indices.size();
	// ���������� VAO
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coord));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	// "�������" ������ VAO, ���� �������� �� ���������
    glBindVertexArray(0);

    // �������� ���������� ���������
    return true;
}

// ����� ������
// ������������ ������� ������ � ����� ������������� �������� uniform-����������
void Mesh::drawOne()
{
	if (vertexCount == 0) return;
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// ����� ����������������� ���� (������� count �����) 
void Mesh::drawMany(int count)
{
	if (vertexCount == 0) return;
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0, count);
	glBindVertexArray(0);
}
