#ifndef BUFFERS
#define BUFFERS
#pragma once

#include <glad/glad.h>
#include <vector>

// --- Vertex Array Object ---
class VAO
{
private:
	GLuint m_id;

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
	GLuint m_id;
	GLenum m_buffer_type;

protected:
	Buffer();
	Buffer(GLenum buffer_type);

public:
	~Buffer();
	void bind() const;
	void unbind() const;
	template<typename T>
	void setData(const std::vector<T> &data)
	{
		bind();
		const T *arr = &data[0];
		glBufferData(m_buffer_type, sizeof(T)*data.size(), arr, GL_STATIC_DRAW);
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
