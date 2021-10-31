#include "buffers.h"

// --- Vertex Array Object abstraction class ---
VAO::VAO()
{
	glGenVertexArrays(1, &this->id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &this->id);
}

void VAO::bind() const 
{
	glBindVertexArray(this->id);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

GLuint VAO::getHandle() const
{
	return this->id;
}


// --- Buffers ---

Buffer::Buffer() : buffer_type(GL_ARRAY_BUFFER)
{
	glGenBuffers(1, &this->id);
}

Buffer::Buffer(GLenum buffer_type) : buffer_type(buffer_type)
{
	glGenBuffers(1, &this->id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &this->id);
}

void Buffer::bind() const
{
	glBindBuffer(this->buffer_type, this->id);
}

void Buffer::unbind() const
{
	glBindBuffer(this->buffer_type, 0);
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