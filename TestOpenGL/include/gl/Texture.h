
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "core/Utils.h"


class CTexture
{
	friend class CTextureFactory;
public:
	unsigned int getId() const;
	const CSize& getSize() const;
	const std::string& getFileName() const;

private:
	CTexture();
	~CTexture();

	unsigned int m_id;
	CSize m_size;
	std::string m_fileName;
}; // class CTexture

#endif // TEXTURE_H