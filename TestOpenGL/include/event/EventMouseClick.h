#ifndef EVENTMOUSECLICK_H
#define EVENTMOUSECLICK_H

#include "AbstractEvent.h"
#include "core/Utils.h"

enum class EMouseButton : unsigned char
{
	left,
	right,
	middle
}; // enum EMouseButton


enum class EMouseState : unsigned char
{
	down,
	up
}; // enum EMouseState

class CEventMouseClick : public AbstractEvent
{
public:
	explicit CEventMouseClick(int x = 0, int y = 0, EMouseButton btn = EMouseButton::left,
		EMouseState state = EMouseState::down);
	virtual ~CEventMouseClick();

	const CVector2& getCoordinates() const;
	EMouseButton getMouseButton() const;
	EMouseState getMouseState() const;


private:
	EMouseButton m_btn;
	EMouseState m_state;
	CVector2 m_coord;
}; // class CEventMouseClick

#endif // EVENTMOUSECLICK_H