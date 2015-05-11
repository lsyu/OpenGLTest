#include "gui/GUIFactory.h"
#include "gui/GUILayer.h"
#include "entities/Rectangle.h"

CGUIFactory::CGUIFactory() : CSingleton<CGUIFactory>(),
	m_entities(),
	m_entities4Event()
{
}

CGUIFactory::~CGUIFactory()
{
	for (auto item : m_entities)
		delete item;
}

void CGUIFactory::addEntity4Event(CBasicEntity* entity)
{
	m_entities4Event.push_back(entity);
}

bool CGUIFactory::hasEntities4Event() const
{
	return !m_entities4Event.empty();
}

CBasicEntity* CGUIFactory::getEntity4Event()
{
	return hasEntities4Event() ? m_entities4Event.back() : nullptr;
}

void CGUIFactory::clearEntity4Event()
{
	m_entities4Event.clear();
}
