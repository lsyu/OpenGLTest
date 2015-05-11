#include "gl/TextureFactory.h"
#include "core/Application.h"

#include "gl/glew.h"
#include "gl/freeglut.h"
#include "IL/il.h"

#include <algorithm>
#include <iostream>

CTextureFactory::CTextureFactory() : CSingleton<CTextureFactory>(),
	m_textures([](CTexture* first, CTexture* second){return first->getFileName() < second->getFileName(); })
{
}

CTextureFactory::~CTextureFactory()
{
	for (auto it = m_textures.begin(), end = m_textures.end();
		it != end; ++it)
	{
		unsigned int id = (*it)->getId();
		glDeleteTextures(1, &id);
		delete *it;
	}
}

CTexture *CTextureFactory::createTexture2D(const std::string &filename)
{
	CTexture *texture = nullptr;

	const std::string& path = CApplication::getInstance()->getApplicationFolder() + "images\\" + filename;

	unsigned int imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	if (ilLoadImage((ILstring)path.c_str()))
	{
		texture = new CTexture;
		texture->m_fileName = filename;

		//ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		glGenTextures(1, &texture->m_id);

		glBindTexture(GL_TEXTURE_2D, texture->m_id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		texture->m_size = { float(ilGetInteger(IL_IMAGE_WIDTH)), float(ilGetInteger(IL_IMAGE_HEIGHT)) };
	}

	ilDeleteImages(1, &imageID);

	return texture;
}

CTexture *CTextureFactory::loadTexture(const std::string &fileName)
{
	auto it = std::find_if(m_textures.begin(), m_textures.end(), [fileName](CTexture* texture) {return texture->getFileName() == fileName; });
	if (it != m_textures.end())
		return *it;

	CTexture *t = createTexture2D(fileName);
	if (t)
		m_textures.insert(t);
	glBindTexture(GL_TEXTURE_2D, 0);

	return t;
}
