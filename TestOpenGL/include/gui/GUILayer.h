#ifndef BASICGUILAYER_H
#define BASICGUILAYER_H

#include "AbstractLayer.h"

class CBasicEntity;
class CGUILayer : public AbstractLayer
{
public:
	explicit CGUILayer();
	virtual ~CGUILayer();


public:
	virtual void prepareGL() override;
	virtual bool updateGL() override;
	virtual bool updateGL(CEventMouseClick *e) override final;
	virtual void paintGL() override;

protected:
	CBasicEntity *m_root;
}; // class CBasicGUILayer

#endif // BASICGUILAYER_H