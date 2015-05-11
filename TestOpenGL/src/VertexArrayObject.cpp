#include "gl/VertexArrayObject.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

#include <iostream>

CVertexArrayObject::CVertexArrayObject() : m_vao(0)
{
}

CVertexArrayObject::~CVertexArrayObject()
{
	glDeleteVertexArrays(1, &m_vao);
}

void CVertexArrayObject::genBuffer()
{
	if (glGenVertexArrays)
	{
		fprintf(stdout, "glGenVertexArrays\n");
		glGenVertexArrays(1, &m_vao);
	}
}

void CVertexArrayObject::bind() const
{
	glBindVertexArray(m_vao);
}

void CVertexArrayObject::disable() const
{
	glBindVertexArray(0);
}
