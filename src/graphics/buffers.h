#ifndef BUFFERS
#define BUFFERS
#pragma once

#include <glad/glad.h>

// --- Vertex Array Object ---
class VAO
{
private:
	GLuint id;

public:
	VAO();
	~VAO();
	void bind() const;
	void unbind() const;
	GLuint getHandle() const;
};

// --- Buffers ---
class Buffer
{
private:
	GLuint id;
	GLenum buffer_type;

protected:
	Buffer();
	Buffer(GLenum buffer_type);

public:
	~Buffer();
	void bind() const;
	void unbind() const;
	template<typename T>
	void setData(const T (&data)[])
	{
		bind();
		glBufferData(buffer_type, sizeof(data), data, GL_STATIC_DRAW);
	}
	GLuint getHandle() const;
};

// --- Vertex Buffer Object ---
class VBO : public Buffer
{
public:
	VBO();
};

// --- Element Buffer Object ---
class EBO : public Buffer
{
public:
	EBO();
};

#endif
