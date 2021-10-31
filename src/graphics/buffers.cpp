#include "buffers.h"

// --- Vertex Array Object abstraction class ---
VAO::VAO()
{
	glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
}

void VAO::bind() const 
{
	glBindVertexArray(id);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

GLuint VAO::getHandle() const
{
	return id;
}


// --- Buffers ---

Buffer::Buffer() : buffer_type(GL_ARRAY_BUFFER)
{
	glGenBuffers(1, &id);
}

Buffer::Buffer(GLenum buffer_type) : buffer_type(buffer_type)
{
	glGenBuffers(1, &id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &id);
}

void Buffer::bind() const
{
	glBindBuffer(buffer_type, id);
}

void Buffer::unbind() const
{
	glBindBuffer(buffer_type, 0);
}

GLuint Buffer::getHandle() const
{
	return id;
}

// --- Vertex Buffer Object ---
VBO::VBO() : Buffer(GL_ARRAY_BUFFER)
{
}

// --- Element Buffer Object ---
EBO::EBO() : Buffer(GL_ELEMENT_ARRAY_BUFFER)
{
}