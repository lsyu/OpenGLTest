#include "entities/Lines.h"

#include "gui/GUIFactory.h"
#include "core/Application.h"
#include "gl/Shader.h"
#include "gl/ShaderFactory.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

#include <iostream>	

static int LINESCNT = 0;

CLines::CLines() : CLines(std::string("Lines") + std::to_string(LINESCNT))
{
	LINESCNT++;
}

CLines::CLines(const std::string &id) : CBasicEntity(id),
	m_geometry({ { -1, -1 }, { 2, 2 } }), m_alpha(1.0f)
{
	m_lines.resize(9);

	CGUIFactory* gui_factory = CGUIFactory::getInstance();

	m_rootRect = gui_factory->createElement<CRectangle>({ -1, -1, 2, 2 }, nullptr);
	m_rootRect->setAlpha(0);

	m_lines[0].resize(4);
	m_lines[1].resize(1);
	m_lines[2].resize(3);
	m_lines[3].resize(5);
	m_lines[4].resize(1);
	m_lines[5].resize(5);
	m_lines[6].resize(3);
	m_lines[7].resize(1);
	m_lines[8].resize(4);

	float shiftX = 1.0f / 7.0f;
	float shiftY = 1.0f / 5.0f;

	m_lines[0][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, 4.0f * shiftY, -5.0f * shiftX, 4.0f * shiftY }, this);
	m_lines[0][0]->setColor({1, 0, 0});
	m_lines[0][1] = gui_factory->createElement<CLine>({ -5.0f * shiftX, 4.0f * shiftY, 0.0f * shiftX, -4.0f * shiftY }, this);
	m_lines[0][1]->setColor({ 1, 0, 0 });
	m_lines[0][2] = gui_factory->createElement<CLine>({ 0.0f * shiftX, -4.0f * shiftY, 5.0f * shiftX, 4.0f * shiftY }, this);
	m_lines[0][2]->setColor({ 1, 0, 0 });
	m_lines[0][3] = gui_factory->createElement<CLine>({ 5.0f * shiftX, 4.0f * shiftY, 7.0f * shiftX, 4.0f * shiftY }, this);
	m_lines[0][3]->setColor({ 1, 0, 0 });

	m_lines[1][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, 3.0f * shiftY, 7.0f * shiftX, 3.0f * shiftY }, this);
	m_lines[1][0]->setColor({ 0, 1, 0 });

	m_lines[2][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, 2.0f * shiftY, -3.0f * shiftX, 2.0f * shiftY }, this);
	m_lines[2][0]->setColor({ 0, 0, 1 });
	m_lines[2][1] = gui_factory->createElement<CLine>({ -3.0f * shiftX, 2.0f * shiftY, 3.0f * shiftX, -2.0f * shiftY }, this);
	m_lines[2][1]->setColor({ 0, 0, 1 });
	m_lines[2][2] = gui_factory->createElement<CLine>({ 3.0f * shiftX, -2.0f * shiftY, 7.0f * shiftX, -2.0f * shiftY }, this);
	m_lines[2][2]->setColor({ 0, 0, 1 });

	m_lines[3][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, 1.0f * shiftY, -4.0f * shiftX, 1.0f * shiftY }, this);
	m_lines[3][0]->setColor({ 1, 1, 0 });
	m_lines[3][1] = gui_factory->createElement<CLine>({ -4.0f * shiftX, 1.0f * shiftY, -3.0f * shiftX, -4.5f * shiftY }, this);
	m_lines[3][1]->setColor({ 1, 1, 0 });
	m_lines[3][2] = gui_factory->createElement<CLine>({ -3.0f * shiftX, -4.5f * shiftY, 3.0f * shiftX, -4.5f * shiftY }, this);
	m_lines[3][2]->setColor({ 1, 1, 0 });
	m_lines[3][3] = gui_factory->createElement<CLine>({ 3.0f * shiftX, -4.5f * shiftY, 4.0f * shiftX, 1.0f * shiftY }, this);
	m_lines[3][3]->setColor({ 1, 1, 0 });
	m_lines[3][4] = gui_factory->createElement<CLine>({ 4.0f * shiftX, 1.0f * shiftY, 7.0f * shiftX, 1.0f * shiftY }, this);
	m_lines[3][4]->setColor({ 1, 1, 0 });

	m_lines[4][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, 0.0f * shiftY, 7.0f * shiftX, 0.0f * shiftY }, this);
	m_lines[4][0]->setColor({ 0, 1, 1 });

	m_lines[5][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, -1.0f * shiftY, -4.0f * shiftX, -1.0f * shiftY }, this);
	m_lines[5][0]->setColor({ 1, 0, 1 });
	m_lines[5][1] = gui_factory->createElement<CLine>({ -4.0f * shiftX, -1.0f * shiftY, -3.0f * shiftX, 4.5f * shiftY }, this);
	m_lines[5][1]->setColor({ 1, 0, 1 });
	m_lines[5][2] = gui_factory->createElement<CLine>({ -3.0f * shiftX, 4.5f * shiftY, 3.0f * shiftX, 4.5f * shiftY }, this);
	m_lines[5][2]->setColor({ 1, 0, 1 });
	m_lines[5][3] = gui_factory->createElement<CLine>({ 3.0f * shiftX, 4.5f * shiftY, 4.0f * shiftX, -1.0f * shiftY }, this);
	m_lines[5][3]->setColor({ 1, 0, 1 });
	m_lines[5][4] = gui_factory->createElement<CLine>({ 4.0f * shiftX, -1.0f * shiftY, 7.0f * shiftX, -1.0f * shiftY }, this);
	m_lines[5][4]->setColor({ 1, 0, 1 });

	m_lines[6][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, -2.0f * shiftY, -3.0f * shiftX, -2.0f * shiftY }, this);
	m_lines[6][0]->setColor({ 0, 0, 0 });
	m_lines[6][1] = gui_factory->createElement<CLine>({ -3.0f * shiftX, -2.0f * shiftY, 3.0f * shiftX, 2.0f * shiftY }, this);
	m_lines[6][1]->setColor({ 0, 0, 0 });
	m_lines[6][2] = gui_factory->createElement<CLine>({ 3.0f * shiftX, 2.0f * shiftY, 7.0f * shiftX, 2.0f * shiftY }, this);
	m_lines[6][2]->setColor({ 0, 0, 0 });

	m_lines[7][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, -3.0f * shiftY, 7.0f * shiftX, -3.0f * shiftY }, this);
	m_lines[7][0]->setColor({ 0, 0, 0 });

	m_lines[8][0] = gui_factory->createElement<CLine>({ -7.0f * shiftX, -4.0f * shiftY, -5.0f * shiftX, -4.0f * shiftY }, this);
	m_lines[8][0]->setColor({ 0.5, 0.5, 0.5 });
	m_lines[8][1] = gui_factory->createElement<CLine>({ -5.0f * shiftX, -4.0f * shiftY, 0.0f * shiftX, 4.0f * shiftY }, this);
	m_lines[8][1]->setColor({ 0.5, 0.5, 0.5 });
	m_lines[8][2] = gui_factory->createElement<CLine>({ 0.0f * shiftX, 4.0f * shiftY, 5.0f * shiftX, -4.0f * shiftY }, this);
	m_lines[8][2]->setColor({ 0.5, 0.5, 0.5 });
	m_lines[8][3] = gui_factory->createElement<CLine>({ 5.0f * shiftX, -4.0f * shiftY, 7.0f * shiftX, -4.0f * shiftY }, this);
	m_lines[8][3]->setColor({ 0.5, 0.5, 0.5 });

	for (size_t index = 1, n = m_lines.size(); index < n; ++index)
	{
		setVisible(index, false);
	}
}

CLines::~CLines()
{
	for (const std::vector<CLine*>& lines : m_lines)
	{
		for (CLine* line : lines)
		{
			delete line;
		}
	}
}

void CLines::recalculateBBox()
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
}

void CLines::configure()
{
	recalculateBBox();

	for (auto child : m_childs)
	{
		child->configure();
	}
}

void CLines::paint()
{
	for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->paint();
	}
}

CBoundingBox CLines::getBoundingBox() const
{
	return m_geometry;
}

void CLines::setGeometry(float x, float y, float width, float height)
{
	m_geometry = { { x, y }, { width, height } };
}

void CLines::prepareEvent(const CEventMouseClick &event)
{
	/*for (int i = m_childs.size() - 1; i >= 0; --i)
	{
		m_childs[i]->prepareEvent(event);
	}*/
}

void CLines::setVisible(size_t index, bool is_visible)
{
	for (CLine* line : m_lines[index])
	{
		line->setAlpha(is_visible ? 1.0f : 0.0f);
	}
}

void CLines::setNextState()
{
	static size_t state = 0;
	state++;

	for (size_t index = 0, n = m_lines.size(); index < n; ++index)
	{
		setVisible(index, false);
	}
	
	CRandomElement random(9);
	for (size_t i = 0, n = 1 + 2 * (state % 5); i < n; ++i)
	{
		setVisible(random.getElement(i), true);
	}
}
