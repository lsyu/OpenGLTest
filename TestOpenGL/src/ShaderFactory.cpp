#include "gl/ShaderFactory.h"
#include "core/Application.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/freeglut.h"

#include <fstream>
#include <iostream>

CShaderFactory::CShaderFactory() : CSingleton<CShaderFactory>(),
	m_prefix(CApplication::getInstance()->getApplicationFolder() + "shaders\\"),
	m_shaders(), m_activeShader(nullptr)
{
}

CShader *CShaderFactory::getShader(const std::string &name)
{
	std::map< std::string, std::shared_ptr<CShader> >::const_iterator it
		= m_shaders.find(name);
	if (it != m_shaders.end())
		return changeActiveShader(it->second.get());

	std::shared_ptr<CShader> shader = std::shared_ptr<CShader>(new CShader(name));
	if (!prepareShader(shader.get(), m_prefix + name + ".vert", m_prefix + name + ".frag"))
	{
		return nullptr; 
	}

	m_shaders.insert(std::pair< std::string, std::shared_ptr<CShader> >(name, shader));
	return changeActiveShader(shader.get());
}

bool CShaderFactory::prepareShader(CShader *shader, const std::string &vertShaderName, const std::string &fragmentShaderName)
{
	shader->m_shaderProgram = glCreateProgram();

	if (!shader->m_shaderProgram)
		return false;

	std::string vs((std::istreambuf_iterator<char>(*(std::auto_ptr<std::ifstream>(
		new std::ifstream(vertShaderName))).get())), std::istreambuf_iterator<char>());
	int size = vs.size();
	if (!makeShader(shader, EShaderType::VertexShader, vs.c_str(), &size))
		return false;

	std::string fs((std::istreambuf_iterator<char>(*(std::auto_ptr<std::ifstream>(
		new std::ifstream(fragmentShaderName))).get())), std::istreambuf_iterator<char>());
	size = fs.size();
	if (!makeShader(shader, EShaderType::FragmentShader, fs.c_str(), &size))
		return false;

	glLinkProgram(shader->m_shaderProgram);

	if (!checkLinkStatus(shader))
		return false;

	//glValidateProgram(shader->m_shaderProgram);
	return true;// checkValidateStatus(shader);
}

bool CShaderFactory::makeShader(CShader *shader, EShaderType type, const char *source, int *sizes)
{
	if (type == EShaderType::VertexShader) {
		shader->m_vertexShader = glCreateShader(GL_VERTEX_SHADER);

		if (!shader->m_vertexShader)
		{
			return false;
		}

		glShaderSource(shader->m_vertexShader, 1, &source, sizes);
		glCompileShader(shader->m_vertexShader);

		if (!checkCompileShaderStatus(shader, shader->m_vertexShader))
		{
			return false;
		}

		appendShader(shader, EShaderType::VertexShader);
	}
	else if (type == EShaderType::FragmentShader) {
		shader->m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		if (!shader->m_fragmentShader)
			return false;

		glShaderSource(shader->m_fragmentShader, 1, &source, sizes);
		glCompileShader(shader->m_fragmentShader);

		if (!checkCompileShaderStatus(shader, shader->m_fragmentShader))
			return false;

		appendShader(shader, EShaderType::FragmentShader);
	}
	return true;
}

void CShaderFactory::appendShader(CShader *shader, EShaderType type)
{
	if (type == EShaderType::VertexShader)
		glAttachShader(shader->m_shaderProgram, shader->m_vertexShader);
	else if (type == EShaderType::FragmentShader)
		glAttachShader(shader->m_shaderProgram, shader->m_fragmentShader);
}

bool CShaderFactory::checkCompileShaderStatus(CShader *program, unsigned int shader)
{
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		handleError(program, shader);
		return false;
	}
	return true;
}

bool CShaderFactory::checkLinkStatus(CShader *shader)
{
	int status;
	glGetProgramiv(shader->m_shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		handleError(shader, shader->m_shaderProgram);
		return false;
	}
	return true;
}

bool CShaderFactory::checkValidateStatus(CShader *shader)
{
	int status;
	glGetProgramiv(shader->m_shaderProgram, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE) {
		handleError(shader, shader->m_shaderProgram);
		return false;
	}
	return true;
}

void CShaderFactory::handleError(CShader *program, unsigned int shader)
{
	//! TODO: Залогировать!
	//! TODO: Вывод в лог ошибки, что за нах
	int maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	std::vector<char> infoLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

	glDeleteProgram(program->m_shaderProgram);
	program->m_shaderProgram = 0;
	glDeleteShader(program->m_vertexShader);
	program->m_vertexShader = 0;
	glDeleteShader(program->m_fragmentShader);
	program->m_fragmentShader = 0;

	std::string out;
	for (int i = 0, n = infoLog.size(); i < n; ++i)
		out.push_back( infoLog[i]);

	fprintf(stdout, out.c_str());
}

CShader *CShaderFactory::changeActiveShader(CShader *newShader)
{
	if (m_activeShader && m_activeShader->m_id != newShader->m_id) {
		m_activeShader->disable();
		m_activeShader = newShader;
		m_activeShader->bind();
	}
	else {
		m_activeShader = newShader;
		m_activeShader->bind();
	}
	return m_activeShader;
}
