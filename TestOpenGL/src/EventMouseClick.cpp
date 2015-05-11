#include "event/EventMouseClick.h"
#include "core/Application.h"

CEventMouseClick::CEventMouseClick(int x, int y, EMouseButton btn, EMouseState state) : AbstractEvent(),
	m_coord(), m_btn(btn), m_state(state)
{
	CVector2 coord = CApplication::getInstance()->getRelativeCoordinateFromAbsolute({ float(x), float(y) });
	this->m_coord = { coord.x, -coord.y };
}

CEventMouseClick::~CEventMouseClick()
{
}

const CVector2& CEventMouseClick::getCoordinates() const
{
	return m_coord;
}

EMouseButton CEventMouseClick::getMouseButton() const
{
	return m_btn;
}

EMouseState CEventMouseClick::getMouseState() const
{
	return m_state;
}