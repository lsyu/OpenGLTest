#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include "core/Singleton.h"
#include "core/Utils.h"
#include <vector>

class CBasicEntity;
class CBasicEntity;
class CRectangle;
class CGUILayer;

class CGUIFactory : public CSingleton < CGUIFactory >
{
	friend class CSingleton<CGUIFactory>;
	friend class CApplication;
public:
	
	template<typename T>
	T* createElement(CBoundingBox bbox, CBasicEntity* parent = nullptr)
	{
		T* ret = new T;
		if (parent)
			parent->addChild(ret);
		ret->setGeometry(bbox.minimum.x, bbox.minimum.y, bbox.maximum.x, bbox.maximum.y);
		return ret;
	}

	void addEntity4Event(CBasicEntity* entity);
	bool hasEntities4Event() const;
	CBasicEntity* getEntity4Event();
	void clearEntity4Event();

private:
	CGUIFactory();
	virtual ~CGUIFactory();
	CGUILayer* createGuiLayer();

	std::vector<CBasicEntity*> m_entities;
	std::vector<CBasicEntity*> m_entities4Event;
}; // class CGUIFactory

#endif // GUIFACTORY_H