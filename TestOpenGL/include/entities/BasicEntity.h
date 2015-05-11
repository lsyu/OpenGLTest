#ifndef BASICENTITY_H
#define BASICENTITY_H

#include "entities/AbstractEntity.h"

#include "gl/vertexbufferobject.h"
#include "gl/vertexarrayobject.h"

#include "core/Utils.h"

#include <vector>

class CEventMouseClick;
class CApplication;
class DefaultScene;

class CBasicEntity : public AbstractEntity
{
	friend class CGUILayer;
	friend class CGUIFactory;
	friend class CRectangle;
	friend class CRectangleAnimated;
	friend class CLine;

public:

	struct CVertex2D
	{
		CVector2 vertex;
		CVector2 uv;
		CColor color;
	}; // struct CVertex3D

	typedef std::vector<CVertex2D> CVertices2D;
	typedef std::vector<CBasicEntity*> CChilds2D;

	// AbstractEntity interface
public:
	virtual void configure() override;
	virtual void paint() override;
	virtual const std::string& getId() const override final;

public:
	void setId(const std::string &id);

	// parent-childs
public:
	bool isRoot() const;

	virtual void setParent(CBasicEntity *m_parent);
	virtual CBasicEntity *getParent() const;

	virtual void addChild(CBasicEntity *child);
	virtual CBasicEntity *getChild(const std::string &id) final;
	virtual CChilds2D& getChilds() final;

	// gui stuff
public:
	
	virtual CBoundingBox getBoundingBox() const = 0;
	virtual void onClicked(const CEventMouseClick &event) {}
	virtual void onPressed(const CEventMouseClick &event) {}
	virtual void onReleased(const CEventMouseClick &event) {}

protected:
	CBasicEntity();
	explicit CBasicEntity(const std::string &id);
	virtual ~CBasicEntity();

	virtual void prepareEvent(const CEventMouseClick &event) = 0;
	virtual void recalculateBBox() {}

	std::string m_id;
	CChilds2D m_childs;
	CBasicEntity *m_parent;

	CVertices2D m_vertices;

	CVertexArrayObject m_vao;
	CVertexBufferObject m_vertexVBO;
}; // class BasicEntity


#endif // BASICENTITY_H