#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <map>

#include "core/Utils.h"

class CShader
{
	friend class CShaderFactory;
public:
	~CShader();
	
	unsigned int setAttribute(const std::string &nameOfParam, int numberOfElement, size_t offsetFromBeginToElements, size_t sizeOfStruct);
	void setAttribute(unsigned int location, int numberOfElement, size_t offsetFromBeginToElements, size_t sizeOfStruct);

	void setUniform(const std::string &nameOfParam, int value);
	void setUniform(const std::string &nameOfParam, float value);
	void setUniform(const std::string &nameOfParam, const CVector2 &vec);
	void setUniform(const std::string &nameOfParam, const CSize &size);
	void setUniform(const std::string &nameOfParam, const CColor &color);
	void setUniform(const std::string &nameOfParam, const CBoundingBox &bbox);

	int getUniformLocation(const std::string &nameOfParam);
	unsigned int getAttributeLocation(const std::string &nameOfParam);

private:
	CShader(const std::string &m_id);


	void bind() const;
	void disable() const;

	std::map<std::string, int> m_cacheAttribute;
	std::map<std::string, int> m_cacheUniform;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
	std::string m_id;
}; // class CShader

#endif // SHADER_H