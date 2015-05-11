#include "entities/Line.h"

#include "gui/GUIFactory.h"
#include "core/Application.h"
#include "gl/Shader.h"
#include "gl/ShaderFactory.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

#include <iostream>

static int LINECNT = 0;

CLine::CLine() : CLine(std::string("Line") + std::to_string(LINECNT))
{
	LINECNT++;
}

CLine::CLine(const std::string &id) : CBasicEntity(id),
	m_geometry({ { -1, -1 }, { 1, 1 } }), m_alpha(1.0f)
{
	//m_vertices.reserve(2);
	m_vertices = {
		//|  position       |  uv             | colors
		{ { -1.0f, -1.0f }, { -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 1.0f },   { -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f } } };
}

CLine::~CLine()
{
}

void CLine::recalculateBBox()
{
	if (m_parent)
	{
		const CBoundingBox& parent_size = m_parent->getBoundingBox(); // size of rectangle
		const CBoundingBox& parent_bbox = {parent_size.minimum.x, parent_size.minimum.y, parent_size.minimum.x + parent_size.maximum.x, parent_size.minimum.y + parent_size.maximum.y}; // bbox of rectangle

		m_geometry.minimum.x = parent_bbox.minimum.x + m_geometry.minimum.x * parent_size.maximum.x * 0.5f + parent_size.maximum.x*0.5f;
		m_geometry.minimum.y = parent_bbox.minimum.y + m_geometry.minimum.y * parent_size.maximum.y * 0.5f + parent_size.maximum.y*0.5f;
		m_geometry.maximum.x = parent_bbox.minimum.x + m_geometry.maximum.x * parent_size.maximum.x * 0.5f + parent_size.maximum.x*0.5f;
		m_geometry.maximum.y = parent_bbox.minimum.y + m_geometry.maximum.y * parent_size.maximum.y * 0.5f + parent_size.maximum.y*0.5f;
	}

	m_vertices[0].vertex = { m_geometry.minimum.x, m_geometry.minimum.y};
	m_vertices[1].vertex = { m_geometry.maximum.x, m_geometry.maximum.y };
}

void CLine::configure()
{
	recalculateBBox();

	CShader *shader = CShaderFactory::getInstance()->getShader("gui_color");
	if (shader)
	{
		m_vertexVBO.genBuffer();
		m_vertexVBO.setData(&m_vertices);

		shader->setAttribute("a_position", 2, 0, sizeof(CVertex2D));
		//shader->setAttribute("a_uv", 2, sizeof(CVertex2D), sizeof(CVertex2D));
		shader->setAttribute("a_color", 3, sizeof(CVertex2D) * 2, sizeof(CColor));

		m_vertexVBO.disable();
	}

	for (auto child : m_childs)
	{
		child->configure();
	}
}

void CLine::paint()
{
	CShader *shader = CShaderFactory::getInstance()->getShader("gui_color");

	if (shader && m_alpha > 0.0f)
	{
		shader->setUniform("alpha", m_alpha);

		m_vertexVBO.bind();

		unsigned int a_position = shader->getAttributeLocation("a_position");
		//unsigned int a_uv = shader->getAttributeLocation("a_uv");
		unsigned int a_color = shader->getAttributeLocation("a_color");

		glEnableVertexAttribArray(a_position);
		//glEnableVertexAttribArray(a_uv);
		glEnableVertexAttribArray(a_color);

		glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + 0);
		//glVertexAttribPointer(a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + sizeof(CVector2));
		glVertexAttribPointer(a_color, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + sizeof(CVector2) * 2);

		glLineWidth(2);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
		glLineWidth(1);

		m_vertexVBO.disable();
	}

	for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->paint();
	}
}

CBoundingBox CLine::getBoundingBox() const
{
	return m_geometry;
}

void CLine::setGeometry(float x, float y, float width, float height)
{
	m_geometry = { { x, y }, { width, height } };
}

void CLine::setColor(const CColor &color)
{
	m_vertices[0].color = m_vertices[1].color = color;
}

void CLine::setAlpha(float alpha)
{
	this->m_alpha = alpha;
}

void CLine::prepareEvent(const CEventMouseClick &event)
{
	for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->prepareEvent(event);
	}
}
