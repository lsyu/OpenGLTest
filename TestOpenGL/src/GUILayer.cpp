#include "gui/GUILayer.h"
#include "gui/GUIFactory.h"
#include "entities/BasicEntity.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"
#include "event/EventMouseClick.h"

CGUILayer::CGUILayer() : AbstractLayer(),
	m_root(nullptr)
{
}

CGUILayer::~CGUILayer()
{
}

void CGUILayer::prepareGL()
{
	// m_root инициализируется здесь, потому что только к этому моменту можно сказать,
	// что контекст OpenGl создан.
	if (m_root)
		m_root->configure();
}

bool CGUILayer::updateGL()
{
	return true;
}

bool CGUILayer::updateGL(CEventMouseClick *e)
{
	if (m_root)
	{
		static CBasicEntity *entityDown = nullptr, *entityUp = nullptr;
		EMouseState state = e->getMouseState();
		m_root->prepareEvent(*e);

		if (CGUIFactory::getInstance()->hasEntities4Event()) 
		{
			if (state == EMouseState::down)
			{
				entityDown = CGUIFactory::getInstance()->getEntity4Event();
				entityUp = nullptr;
				entityDown->onPressed(*e);
			}
			else
			{
				entityUp = CGUIFactory::getInstance()->getEntity4Event();
				entityUp->onReleased(*e);

				if (entityUp && entityDown && entityDown->getId() == entityUp->getId())
				{
					entityUp->onClicked(*e);
					entityUp = entityDown = nullptr;
				}
			}
		}
	}
	return true;
}

void CGUILayer::paintGL()
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_root)
		m_root->paint();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
