#include "gl/VertexBufferObject.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"


CVertexBufferObject::CVertexBufferObject(EArrayType type) : m_vbo(0), m_type(type)
{
}

CVertexBufferObject::~CVertexBufferObject()
{
	if (m_vbo >= 0)
		glDeleteBuffers(1, &m_vbo);
}

void CVertexBufferObject::genBuffer()
{
	glGenBuffers(1, &m_vbo);
}

void CVertexBufferObject::bind() const
{
	glBindBuffer(m_type == EArrayType::Data ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, m_vbo);
}

void CVertexBufferObject::disable() const
{
	glBindBuffer(m_type == EArrayType::Data ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CVertexBufferObject::setData(const void *data, size_t sizeOfData) const
{
	bind();
	glBufferData(m_type == EArrayType::Data ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER,
		sizeOfData, data, GL_STATIC_DRAW);
}
