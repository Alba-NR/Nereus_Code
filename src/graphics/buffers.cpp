#include "buffers.h"

#include <iostream>

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


// --- Framebuffer Object ---

FBO::FBO()
	: m_colour_attachment()
{
	this->create();
}

FBO::FBO(Texture2D &colour_texture)
	: m_colour_attachment(colour_texture)
{
	this->create();
}

// Create and setup the FBO
void FBO::create()
{
	// generate FBO
	glGenFramebuffers(1, &m_id);

	// create renderbuffer object for depth & stencil attachments
	glGenRenderbuffers(1, &m_rbo_depth_stencil_attachment);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_depth_stencil_attachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_colour_attachment.getWidth(), m_colour_attachment.getHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind RBO

	// --- attachments ---
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	// attach tex as colour attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colour_attachment.getHandle(), 0);

	// attach rbo as depth & stencil attachments
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_depth_stencil_attachment);

	// --- check if fbo is complete ---
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FBO::INCOMPLETE_FBO" << std::endl
			<< "FBO with handle " << m_id << " is incomplete " << std::endl;
	}

	// --- unbind framebuffer ---
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
	glDeleteRenderbuffers(1, &m_rbo_depth_stencil_attachment);
	glDeleteFramebuffers(1, &m_id);
}

void FBO::setColourAttachment(Texture2D &colour_texture)
{
	m_colour_attachment = colour_texture;
	// update fbo attachment
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colour_attachment.getHandle(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture2D &FBO::getColourAttachment()
{
	return m_colour_attachment;
}


void FBO::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FBO::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FBO::getHandle() const
{
	return m_id;
}
