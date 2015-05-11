#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "BasicEntity.h"

#include "gl/VertexBufferObject.h"
#include "gl/VertexArrayObject.h"

#include "core/Utils.h"

class CTexture;
class CRectangle : public CBasicEntity
{
	friend class CGUIFactory;
public:
	CRectangle();
	explicit CRectangle(const std::string &m_id);
	virtual ~CRectangle();

	// AbstractEntity interface
public:
	virtual void paint() override;
	virtual void configure() override;

	// CBasicEntity interface
public:
	virtual CBoundingBox getBoundingBox() const override;
	//virtual void onClicked(const CEventMouseClick &event) override;
protected:
	virtual void prepareEvent(const CEventMouseClick &event) override;
	virtual void recalculateBBox() override;

public:
	void setGeometry(float x, float y, float width, float height);
	void setColor(const CColor& color);
	void setTexture(CTexture* texture);
	void setAlpha(float alpha);
	void setLabel(const std::string& label);

protected:
	bool contains(const CVector2& point) const;

	CBoundingBox m_geometry; // x,y,width,height
	struct CBackground
	{
		unsigned char type = 0; // 0 - not init, 1 - color, 2 - texture
		union CBackgroundImpl
		{
			CTexture *m_texture = nullptr;
			CColor *m_color;
		} impl;
	} m_background;
	float m_alpha;
	std::string m_label;
}; //class Rectangle


#endif // RECTANGLE_H