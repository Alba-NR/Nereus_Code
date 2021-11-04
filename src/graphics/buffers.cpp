#include "buffers.h"

// --- Vertex Array Object abstraction class ---
VAO::VAO()
{
	glGenVertexArrays(1, &m_id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_id);
}

void VAO::bind() const 
{
	glBindVertexArray(m_id);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

GLuint VAO::getHandle() const
{
	return m_id;
}


// --- Buffers ---

Buffer::Buffer() : m_buffer_type(GL_ARRAY_BUFFER)
{
	glGenBuffers(1, &m_id);
}

Buffer::Buffer(GLenum buffer_type) : m_buffer_type(buffer_type)
{
	glGenBuffers(1, &m_id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}

void Buffer::bind() const
{
	glBindBuffer(m_buffer_type, m_id);
}

void Buffer::unbind() const
{
	glBindBuffer(m_buffer_type, 0);
}

GLuint Buffer::getHandle() const
{
	return m_id;
}

// --- Vertex Buffer Object ---
VBO::VBO() : Buffer(GL_ARRAY_BUFFER)
{
}

// --- Element Buffer Object ---
EBO::EBO() : Buffer(GL_ELEMENT_ARRAY_BUFFER)
{
}