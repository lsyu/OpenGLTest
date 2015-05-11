#include "gl/Texture.h"

CTexture::CTexture() : m_id(0), m_size(), m_fileName()
{
}

CTexture::~CTexture()
{
}

unsigned int CTexture::getId() const
{
	return m_id;
}

const CSize& CTexture::getSize() const
{
	return m_size;
}

const std::string& CTexture::getFileName() const
{
	return m_fileName;
}
