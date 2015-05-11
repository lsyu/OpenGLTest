#ifndef RECTANGLEANIMATEDUV_H
#define RECTANGLEANIMATEDUV_H

#include "Rectangle.h"

class CRectangleAnimatedUV : public CRectangle
{
	friend class CGUIFactory;
public:
	CRectangleAnimatedUV();

	// AbstractEntity interface
public:
	virtual void configure() override;
	virtual void paint() override;

public:
	void setRPS(size_t rps, float time_running);
	bool isRunning() const;
	void run();

	size_t m_uvRPS;
	mutable float m_timeElapsed;
	float m_timeRunning;
	mutable bool m_isRunning;
}; //class CRectangleAnimatedUV


#endif // RECTANGLEANIMATEDUV_H