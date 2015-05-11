#ifndef LINES_H
#define LINES_H

#include "entities/BasicEntity.h"
#include "entities/Rectangle.h"
#include "entities/Line.h"

#include "gl/VertexBufferObject.h"
#include "gl/VertexArrayObject.h"

#include "core/Utils.h"

#include <vector>

class CLines : public CBasicEntity
{
	friend class CGUIFactory;
public:
	explicit CLines();
	explicit CLines(const std::string &m_id);
	virtual ~CLines();

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
	void setVisible(size_t index, bool is_visible);
	void setNextState();

protected:

	CBoundingBox m_geometry; // x_from,y_from,width,height
	float m_alpha;
	CRectangle* m_rootRect;
	std::vector<std::vector<CLine*>> m_lines;
}; //class CLines


#endif // LINES_H