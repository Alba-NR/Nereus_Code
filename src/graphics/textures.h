#ifndef TEXTURES
#define TEXTURES
#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

using std::string;

// --- 2D Texture ---
class Texture2D
{
private:
	GLuint m_id;
	int m_width;
	int m_height;
	int m_num_channels;

public:
	Texture2D(string filename);
	Texture2D();
	Texture2D(int width, int height);
	~Texture2D();

	void bind() const;
	GLuint getHandle() const;

	int getWidth() const;
	int getHeight() const;
};

// --- Cubemap Texture ---
class CubeMapTexture
{
private:
	GLuint m_id;

public:
	CubeMapTexture(const string filenames[6]);
	~CubeMapTexture();

	void bind() const;
	GLuint getHandle() const;
};
#endif