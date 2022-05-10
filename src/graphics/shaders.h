#ifndef SHADERS
#define SHADERS
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "buffers.h"

using std::string;


class Shader
{
private:
	string m_filename;
	GLenum m_type;
	GLuint m_id;

	void loadAndCompile();

public:
	Shader(string filename);
	~Shader();
	GLuint getHandle() const;
	string getFilename() const;
};

class ShaderProgram
{
private:
	std::vector<Shader> m_shaders;
	GLuint m_id;
	string m_str_name;

	void createProgram();

public:
	ShaderProgram(const std::vector<Shader> &shaders);
	~ShaderProgram();
	void use();
	GLuint getHandle() const;

	void bindData(GLuint location, const Buffer &buffer, GLuint attribute_size) const;

	void setInt(const string &target, int v) const;
	void setFloat(const string &target, float v) const;
	void setVec2(const string &target, const glm::vec2 &v) const;
	void setVec3(const string &target, const glm::vec3 &v) const;
	void setVec4(const string &target, const glm::vec4 &v) const;
	void setMat3(const string &target, const glm::mat3 &v) const;
	void setMat4(const string &target, const glm::mat4 &v) const;
	void setIntArray(const string &target, int v[], int length) const;
	void setFloatArray(const string &target, float v[], int length) const;
	void setVec2Array(const string &target, glm::vec2 v[], int length) const;
};

#endif
