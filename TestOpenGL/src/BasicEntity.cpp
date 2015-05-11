#include "entities/BasicEntity.h"
#include <algorithm>

static int ENTITYCNT = 0;

CBasicEntity::CBasicEntity() : CBasicEntity(std::string("entity") + std::to_string(ENTITYCNT))
{
	ENTITYCNT++;
}

CBasicEntity::CBasicEntity(const std::string &id) : AbstractEntity(), m_id(id), m_childs(),
	m_parent(nullptr), m_vertices(), m_vao(), m_vertexVBO()
{
}

CBasicEntity::~CBasicEntity()
{
}

void CBasicEntity::paint()
{
}

const std::string& CBasicEntity::getId() const
{
	return m_id;
}

void CBasicEntity::configure()
{
}

void CBasicEntity::setId(const std::string &id)
{
	this->m_id = id;
}

void CBasicEntity::setParent(CBasicEntity *parent)
{
	this->m_parent = parent;
	if (!parent->getChild(this->m_id))
	{
		parent->addChild(this);
	}
}

CBasicEntity *CBasicEntity::getParent() const
{
	return m_parent;
}

bool CBasicEntity::isRoot() const
{
	return m_parent == nullptr;
}

void CBasicEntity::addChild(CBasicEntity *child)
{
	m_childs.push_back(child);
	child->setParent(this);
}

CBasicEntity *CBasicEntity::getChild(const std::string &id)
{
	std::vector<CBasicEntity *>::const_iterator it = std::find_if(m_childs.begin(), m_childs.end(),
		[&id](CBasicEntity *child) -> bool
	{
		return child->getId() == id;
	});

	if (it != m_childs.end())
		return (CBasicEntity *)(&(*it));

	return nullptr;
}

CBasicEntity::CChilds2D &CBasicEntity::getChilds()
{
	return m_childs;
}
