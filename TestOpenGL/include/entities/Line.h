#ifndef LINE_H
#define LINE_H

#include "BasicEntity.h"

#include "gl/VertexBufferObject.h"
#include "gl/VertexArrayObject.h"

#include "core/Utils.h"

class CLine : public CBasicEntity
{
	friend class CGUIFactory;
public:
	CLine();
	explicit CLine(const std::string &m_id);
	virtual ~CLine();

	// AbstractEntity interface
public:
	virtual void paint() override;
	virtual void configure() override;

	// CBasicEntity interface
public:
	virtual CBoundingBox getBoundingBox() const override;
protected:
	virtual void prepareEvent(const CEventMouseClick &event) override;
	virtual void recalculateBBox() override;

public:
	void setGeometry(float x_from, float y_from, float x_to, float y_to);
	void setColor(const CColor& color);
	void setAlpha(float alpha);

protected:

	CBoundingBox m_geometry; // x_from,y_from,x_to,y_to
	float m_alpha;
}; //class CLine


#endif // LINE_H