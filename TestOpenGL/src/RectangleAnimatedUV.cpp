#include "entities/RectangleAnimatedUV.h"

#include "core/Application.h"
#include "gl/Texture.h"
#include "gl/Shader.h"
#include "gl/ShaderFactory.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

CRectangleAnimatedUV::CRectangleAnimatedUV() : CRectangle(),
	m_uvRPS(5),
	m_timeElapsed(0.0f),
	m_timeRunning(4.0f),
	m_isRunning(false)
{
}

void CRectangleAnimatedUV::configure()
{
	recalculateBBox();

	float step = 1.0f / 3.0f;
	CRandomElement random(5);
	float from = random.getElement(1);
	m_vertices[0].uv = { 0.0f, /*1.0f - */(from + 0.0f) * step };
	m_vertices[1].uv = { 1.0f, /*1.0f - */(from + 0.0f) * step };
	m_vertices[2].uv = { 1.0f, /*1.0f - */(from + 1.0f) * step };
	m_vertices[3].uv = { 0.0f, /*1.0f - */(from + 1.0f) * step };

	CShader *shader = CShaderFactory::getInstance()->getShader("gui_texture");
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

void CRectangleAnimatedUV::paint()
{
	if (m_isRunning)
	{
		m_timeElapsed += CApplication::getInstance()->getTimeLastFrame();

		m_vertexVBO.bind();
		CVertex2D* ptr = (CVertex2D*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		float step = 1.0f / 3.0f;
		
		if (ptr)
		{
			float rpm = float(m_uvRPS) * CApplication::getInstance()->getTimeLastFrame() * step;
			ptr[0].uv.y += rpm;
			ptr[1].uv.y += rpm;
			ptr[2].uv.y += rpm;
			ptr[3].uv.y += rpm;
		}

		if (m_timeElapsed > m_timeRunning)
		{
			if (ptr)
			{
				float rpm = -float(m_uvRPS) * (m_timeElapsed - m_timeRunning) * step;
				ptr[0].uv.y += rpm;
				ptr[1].uv.y += rpm;
				ptr[2].uv.y += rpm;
				ptr[3].uv.y += rpm;
			}

			m_isRunning = false;
			m_timeElapsed = 0.0f;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER); // unmap it after use
		m_vertexVBO.disable();
	}
	
	CShader *shader = CShaderFactory::getInstance()->getShader("gui_texture");

	if (shader && m_alpha > 0.0f)
	{
		shader->setUniform("alpha", m_alpha);

		m_vertexVBO.bind();

		glEnable(GL_TEXTURE_2D);

		unsigned int a_position = shader->getAttributeLocation("a_position");
		unsigned int a_uv = 0;

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_background.impl.m_texture->getId());
		shader->setUniform("texture", 0);

		a_uv = shader->getAttributeLocation("a_uv");

		glEnableVertexAttribArray(a_position);
		glEnableVertexAttribArray(a_uv);

		glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + 0);
		glVertexAttribPointer(a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex2D), (char*)0 + sizeof(CVector2));

		glDrawArrays(GL_QUADS, 0, 4);

		m_vertexVBO.disable();

		glDisable(GL_TEXTURE_2D);
	}

	for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->paint();
	}
}

bool CRectangleAnimatedUV::isRunning() const
{
	return m_isRunning;
}

void CRectangleAnimatedUV::run()
{
	m_timeElapsed = 0.0f;
	m_isRunning = true;
}

void CRectangleAnimatedUV::setRPS(size_t rps, float time_running)
{
	if (!m_isRunning)
	{
		m_uvRPS = rps;
		m_timeRunning = time_running;
	}
}