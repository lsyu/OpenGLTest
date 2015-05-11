#include "entities/Rectangle.h"
#include "gui/GUIFactory.h"
#include "event/EventMouseClick.h"
#include "core/Application.h"
#include "gl/Texture.h"
#include "gl/Shader.h"
#include "gl/ShaderFactory.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

#include <iostream>

static int RECTANGLECNT = 0;

CRectangle::CRectangle() : CRectangle(std::string("Rect") + std::to_string(RECTANGLECNT))
{
	RECTANGLECNT++;
}

CRectangle::CRectangle(const std::string &id) : CBasicEntity(id),
	m_geometry({ {-1, -1}, {2,2} }), m_background(), m_alpha(1.0f),
	m_label()
{
	//m_vertices.reserve(4);
	m_vertices = {
		//|  position       |  uv             | colors
		{ { -1.0f, -1.0f }, { -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 1.0f },  { -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 1.0f },   { -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, -1.0f },  { -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } } };
}

CRectangle::~CRectangle()
{
}

void CRectangle::recalculateBBox()
{
	if (m_parent)
	{
		CBoundingBox parent_bbox = m_parent->getBoundingBox();
		float scWidth = parent_bbox.maximum.x * 0.5f;
		float scHeight = parent_bbox.maximum.y * 0.5f;
		float shiftX = parent_bbox.minimum.x + scWidth;
		float shiftY = parent_bbox.minimum.y + scHeight;

		m_geometry.minimum.x = m_geometry.minimum.x * scWidth + shiftX;
		m_geometry.minimum.y = m_geometry.minimum.y * scHeight + shiftY;
		m_geometry.maximum.x *= scWidth;
		m_geometry.maximum.y *= scHeight;
	}

	m_vertices[0].vertex = { m_geometry.minimum.x, m_geometry.minimum.y };
	m_vertices[1].vertex = { m_geometry.minimum.x + m_geometry.maximum.x, m_geometry.minimum.y };
	m_vertices[2].vertex = { m_geometry.minimum.x + m_geometry.maximum.x, m_geometry.minimum.y + m_geometry.maximum.y };
	m_vertices[3].vertex = { m_geometry.minimum.x, m_geometry.minimum.y + m_geometry.maximum.y };
}

void CRectangle::configure()
{
	recalculateBBox();
	
	if (m_background.type == 2 && m_background.impl.m_texture->getId()) {
		m_vertices[0].uv = { 0.0f, /*1.0f - */0.0f };
		m_vertices[1].uv = { 1.0f, /*1.0f - */0.0f };
		m_vertices[2].uv = { 1.0f, /*1.0f - */1.0f };
		m_vertices[3].uv = { 0.0f, /*1.0f - */1.0f };
	}

	CShader *shader = (m_background.type == 2 && m_background.impl.m_texture->getId())
		? CShaderFactory::getInstance()->getShader("gui_texture")
		: CShaderFactory::getInstance()->getShader("gui_color");
	if (shader)
	{
		m_vertexVBO.genBuffer();
		m_vertexVBO.setData(&m_vertices);

		shader->setAttribute("a_position", 2, 0, sizeof(CVertex2D));
		if (m_background.type == 2 && m_background.impl.m_texture->getId())
		{
			shader->setAttribute("a_uv", 2, sizeof(CVertex2D), sizeof(CVertex2D));
		}
		else
		{
			shader->setAttribute("a_color", 3, sizeof(CVertex2D) * 2, sizeof(CColor));
		}

		m_vertexVBO.disable();
	}

	for (auto child : m_childs)
	{
		child->configure();
	}
}

void CRectangle::paint()
{
	CShader *shader = (m_background.type == 2 && m_background.impl.m_texture->getId())
		? CShaderFactory::getInstance()->getShader("gui_texture")
		: CShaderFactory::getInstance()->getShader("gui_color");

	if (shader && m_alpha > 0.0f)
	{
		shader->setUniform("alpha", m_alpha);

		m_vertexVBO.bind();

		unsigned int a_position = shader->getAttributeLocation("a_position");
		unsigned int a_uv_or_color = 0;
		if (m_background.type == 2 && m_background.impl.m_texture->getId())
		{
			shader->setUniform("texture", 0);
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_background.impl.m_texture->getId());

			a_uv_or_color = shader->getAttributeLocation("a_uv");

			glEnableVertexAttribArray(a_position);
			glEnableVertexAttribArray(a_uv_or_color);

			glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + 0);
			glVertexAttribPointer(a_uv_or_color, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + sizeof(CVector2));
		}
		else
		{
			a_uv_or_color = shader->getAttributeLocation("a_color");

			glEnableVertexAttribArray(a_position);
			glEnableVertexAttribArray(a_uv_or_color);

			glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + 0);
			glVertexAttribPointer(a_uv_or_color, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + sizeof(CVector2) * 2);
		}

		glDrawArrays(GL_QUADS, 0, 4);

		m_vertexVBO.disable();

		if (m_background.type == 2 && m_background.impl.m_texture->getId())
			glDisable(GL_TEXTURE_2D);

		// draw label
		if (!m_label.empty())
		{
			void* font = GLUT_BITMAP_TIMES_ROMAN_24;
			const CSize& app_size = CApplication::getInstance()->getWindowSize();
			const CSize& text_half_size = {
				float(glutBitmapLength(font, (const unsigned char*)m_label.c_str())) / app_size.width,
				float(glutBitmapHeight(font)) / app_size.height * 0.5f
			};
			const CVector2 center = {
				m_geometry.minimum.x + m_geometry.maximum.x * 0.5f,
				m_geometry.minimum.y + m_geometry.maximum.y * 0.5f
			};

			glColor3f(0.1f, 0.1f, 0.1f);
			glRasterPos2d(center.x - text_half_size.width, center.y - text_half_size.height);

			for (char symbol : m_label)
			{
				glutBitmapCharacter(font, symbol);
			}
		}
	}

	for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->paint();
	}
}

bool CRectangle::contains(const CVector2& point) const
{
	if (point.x < m_geometry.minimum.x || point.x > m_geometry.minimum.x + m_geometry.maximum.x)
		return false;
	if (point.y < m_geometry.minimum.y || point.y > m_geometry.minimum.y + m_geometry.maximum.y)
		return false;
	return true;
}

void CRectangle::prepareEvent(const CEventMouseClick &event)
{
	if (contains(event.getCoordinates()))
	{
		CGUIFactory::getInstance()->addEntity4Event(this);
	}

	for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->prepareEvent(event);
	}
}

CBoundingBox CRectangle::getBoundingBox() const
{
	return m_geometry;
}

void CRectangle::setGeometry(float x, float y, float width, float height)
{
	m_geometry = { {x, y}, {width, height} };
}

void CRectangle::setColor(const CColor &color)
{
	m_vertices[0].color = m_vertices[1].color = m_vertices[2].color = m_vertices[3].color = color;
	m_background.impl.m_color = &m_vertices[0].color;
	m_background.type = 1;
}

void CRectangle::setTexture(CTexture* texture)
{
	m_background.impl.m_texture = texture;
	m_background.type = 2;
}

void CRectangle::setAlpha(float alpha)
{
	this->m_alpha = alpha;
}

void CRectangle::setLabel(const std::string& label)
{
	this->m_label = label;
}
