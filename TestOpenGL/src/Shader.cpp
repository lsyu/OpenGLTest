#include "gl/Shader.h"

#include <iostream>

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

CShader::CShader(const std::string &id) : 
	m_cacheAttribute(), m_cacheUniform(), m_vertexShader(0), m_fragmentShader(0),
	m_shaderProgram(0), m_id(id)
{
}

CShader::~CShader()
{
	m_cacheAttribute.clear();
	m_cacheUniform.clear();
	if (m_shaderProgram)
		glDeleteProgram(m_shaderProgram);
	if (m_vertexShader)
		glDeleteShader(m_vertexShader);
	if (m_fragmentShader)
		glDeleteShader(m_fragmentShader);
}

unsigned int CShader::getAttributeLocation(const std::string &nameOfParam)
{
	int location;
	if (m_cacheAttribute.find(nameOfParam) != m_cacheAttribute.end()) {
		location = m_cacheAttribute[nameOfParam];
	}
	else {
		location = glGetAttribLocation(m_shaderProgram, nameOfParam.c_str());
		if (location != -1)
			m_cacheAttribute[nameOfParam] = location;
		else
			std::cout << nameOfParam << " not found!\n"; 
	}
	return static_cast<unsigned int>(location);
}

unsigned int CShader::setAttribute(const std::string &nameOfParam, int numberOfElement, size_t offsetFromBeginToElements, size_t sizeOfStruct)
{
	unsigned int location = getAttributeLocation(nameOfParam);
	glVertexAttribPointer(location, numberOfElement, GL_FLOAT, GL_FALSE, sizeOfStruct, reinterpret_cast<void*>(offsetFromBeginToElements));
	glEnableVertexAttribArray(location);
	return location;
}

void CShader::setAttribute(unsigned int location, int numberOfElement, size_t offsetFromBeginToElements, size_t sizeOfStruct)
{
	glVertexAttribPointer(location, numberOfElement, GL_FLOAT, GL_FALSE, sizeOfStruct, reinterpret_cast<void*>(offsetFromBeginToElements));
	glEnableVertexAttribArray(location);
}

int CShader::getUniformLocation(const std::string &nameOfParam)
{
	int location;
	if (m_cacheUniform.find(nameOfParam) != m_cacheUniform.end()) {
		location = m_cacheUniform[nameOfParam];
	}
	else {
		location = glGetUniformLocation(m_shaderProgram, nameOfParam.c_str());
		if (location != -1)
			m_cacheUniform[nameOfParam] = location;
	}
	return location;
}

void CShader::setUniform(const std::string &nameOfParam, int value)
{
	int location = getUniformLocation(nameOfParam);
	glUniform1i(location, value);
}

void CShader::setUniform(const std::string &nameOfParam, float value)
{
	int location = getUniformLocation(nameOfParam);
	glUniform1f(location, value);
}

void CShader::setUniform(const std::string &nameOfParam, const CVector2 &vec)
{
	int location = getUniformLocation(nameOfParam);
	glUniform2f(location, vec.x, vec.y);
}

void CShader::setUniform(const std::string &nameOfParam, const CSize &vec)
{
	int location = getUniformLocation(nameOfParam);
	glUniform2f(location, vec.width, vec.height);
}

void CShader::setUniform(const std::string &nameOfParam, const CColor &vec)
{
	int location = getUniformLocation(nameOfParam);
	glUniform3f(location, vec.r, vec.g, vec.b);
}

void CShader::setUniform(const std::string &nameOfParam, const CBoundingBox &vec)
{
	int location = getUniformLocation(nameOfParam);
	glUniform4f(location, vec.minimum.x, vec.minimum.y, vec.maximum.x, vec.maximum.y);
}

void CShader::bind() const
{
	glUseProgram(m_shaderProgram);
}

void CShader::disable() const
{
	glUseProgram(0);
}
