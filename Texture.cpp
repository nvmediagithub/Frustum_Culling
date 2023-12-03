#include "Texture.h"

using namespace std;

// загрузка текстуры из внешнего файла
void Texture::load(std::string filename)
{
	cout << "loading texture from " << filename << " ";

	// создаем объект изображения DEVIL
	ILuint img;
	ilGenImages(1, &img);
	ilBindImage(img);

	// загружаем изображение во внутренний объект
	wchar_t Unicode_String[256];
	wsprintf(Unicode_String, L"%S", filename.c_str());
	ilLoadImage(Unicode_String);

	// получение параметров загруженной текстуры
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int type = ilGetInteger(IL_IMAGE_TYPE);
	int fmt = ilGetInteger(IL_IMAGE_FORMAT);
	cout << "("<< width << "x" << height << ")" << endl;

	// выделяем память и копируем массив пикселей
	ILubyte *Data = new ILubyte[width * height * 4];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, Data);

	// создание текстурного объекта
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texIndex);
	glBindTexture(GL_TEXTURE_2D, texIndex);
	// установка параметров текстуры + загрузка
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//установка параметров текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0);

	// удаление ненужных данных
	ilBindImage(0);
	ilDeleteImage(img);
	delete[] Data;
}

// применение текстуры (привязка к текстурному блоку)
void Texture::bind(GLenum texUnit)
{
	// связывание текстурного блока
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, texIndex);
}
