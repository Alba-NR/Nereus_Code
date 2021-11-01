#ifndef SHADERS
#define SHADERS
#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using std::string;


class Shader
{
private:
	string filename;
	GLenum type;
	GLuint id;

	void loadAndCompile();

public:
	Shader(string filename);
	~Shader();
	GLuint getHandle();
};

class ShaderProgram
{
private:
	std::vector<Shader> shaders;
	GLuint id;

	void createProgram();

public:
	ShaderProgram(std::vector<Shader> shaders);
	~ShaderProgram();
	void use();
	GLuint getHandle();

	// TODO: upload to uniforms & bind data methods
};

#endif
