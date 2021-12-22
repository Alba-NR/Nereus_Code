#ifndef BUFFERS
#define BUFFERS
#pragma once

#include "textures.h"

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

// --- Framebuffer Object ---
class FBO
{
private:
	GLuint m_id;
	Texture2D m_colour_attachment;
	GLuint m_rbo_depth_stencil_attachment;

	void create();

public:
	FBO();
	FBO(Texture2D &colour_texture);
	~FBO();

	void setColourAttachment(Texture2D &colour_texture);
	Texture2D &getColourAttachment();

	void bind() const;
	void unbind() const;
	GLuint getHandle() const;
};

#endif
