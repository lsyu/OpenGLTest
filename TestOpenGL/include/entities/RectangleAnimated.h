#ifndef RECTANGLEANIMATED_H
#define RECTANGLEANIMATED_H

#include "entities/Rectangle.h"

class CRectangleAnimated : public CRectangle
{
	friend class CGUIFactory;
public:
	CRectangleAnimated();
	virtual ~CRectangleAnimated();

	// AbstractEntity interface
public:
	virtual void configure() override;
	virtual void paint() override;

public:
	void setTexture1(CTexture* texture);
	void setTexture2(CTexture* texture);

protected:
	CTexture *m_texture1;
	CTexture *m_texture2;
	mutable float m_timeElapsed;
}; //class CRectangleAnimated


#endif // RECTANGLEANIMATED_H