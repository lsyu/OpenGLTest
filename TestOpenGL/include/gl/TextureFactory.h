#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "core/Singleton.h"
#include "gl/Texture.h"
#include "core/Utils.h"

#include <set>
#include <functional>

class CTextureFactory : public CSingleton<CTextureFactory>
{
	friend class CSingleton < CTextureFactory > ;
public:

	CTexture *loadTexture(const std::string &textureName);

private:
	CTextureFactory();
	~CTextureFactory();

	CTexture *createTexture2D(const std::string &filename);

	std::set< CTexture*, std::function<bool(CTexture*, CTexture*)> > m_textures;
}; // class CTextureFactory

#endif // TEXTUREFACTORY_H