#include "entities/RectangleAnimated.h"
#include "core/Application.h"
#include "gl/Texture.h"
#include "gl/Shader.h"
#include "gl/ShaderFactory.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

CRectangleAnimated::CRectangleAnimated() : CRectangle(),
	m_texture1(nullptr),
	m_texture2(nullptr),
	m_timeElapsed(0.0f)
{
}

CRectangleAnimated::~CRectangleAnimated()
{
}

void CRectangleAnimated::setTexture1(CTexture* texture)
{
	m_texture1 = texture;
}

void CRectangleAnimated::setTexture2(CTexture* texture)
{
	m_texture2 = texture;
}

void CRectangleAnimated::configure()
{
	recalculateBBox();

	m_vertices[0].uv = { 0.0f, /*1.0f - */0.0f };
	m_vertices[1].uv = { 1.0f, /*1.0f - */0.0f };
	m_vertices[2].uv = { 1.0f, /*1.0f - */1.0f };
	m_vertices[3].uv = { 0.0f, /*1.0f - */1.0f };

	CShader *shader = CShaderFactory::getInstance()->getShader("gui_texture_animated");
	if (shader)
	{
		m_vertexVBO.genBuffer();
		m_vertexVBO.setData(&m_vertices);

		shader->setAttribute("a_position", 2, 0, sizeof(CVertex2D));
		shader->setAttribute("a_uv", 2, sizeof(CVertex2D), sizeof(CVertex2D));

		m_vertexVBO.disable();
	}

	for (auto child : m_childs)
	{
		child->configure();
	}
}

void CRectangleAnimated::paint()
{
	m_timeElapsed += CApplication::getInstance()->getTimeLastFrame();
	while (m_timeElapsed > 2.0f)
	{
		m_timeElapsed -= 2.0f;
	}
	CShader *shader = CShaderFactory::getInstance()->getShader("gui_texture_animated");

	if (shader && m_alpha > 0.0f)
	{
		shader->setUniform("alpha", m_alpha);
		shader->setUniform("mix_factor", abs(m_timeElapsed - 1.0f));

		m_vertexVBO.bind();

		unsigned int a_position = shader->getAttributeLocation("a_position");
		unsigned int a_uv = 0;
		glEnable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_texture1->getId());
		shader->setUniform("texture1", 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_texture2->getId());
		shader->setUniform("texture2", 1);

		a_uv = shader->getAttributeLocation("a_uv");

		glEnableVertexAttribArray(a_position);
		glEnableVertexAttribArray(a_uv);

		glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + 0);
		glVertexAttribPointer(a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + sizeof(CVector2));

		glDrawArrays(GL_QUADS, 0, 4);

		m_vertexVBO.disable();

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
