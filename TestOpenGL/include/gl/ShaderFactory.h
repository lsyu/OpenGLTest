#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "core/Singleton.h"
#include "gl/Shader.h"

#include <memory>
#include <string>
#include <map>


enum class EShaderType : bool
{
	VertexShader,
	FragmentShader
}; // enum ShaderType

class CShaderFactory : public CSingleton < CShaderFactory >
{
	friend class CSingleton < CShaderFactory > ;
public:
	
	CShader *getShader(const std::string &name);

private:
	CShaderFactory();


	bool prepareShader(CShader *shader, const std::string &vertShaderName, const std::string &fragmentShaderName);
	bool makeShader(CShader *shader, EShaderType type, const char *source, int *sizes);
	void appendShader(CShader *shader, EShaderType type);
	bool checkCompileShaderStatus(CShader *program, unsigned int shader);
	bool checkLinkStatus(CShader *shader);
	bool checkValidateStatus(CShader *shader);
	void handleError(CShader *program, unsigned int shader);
	CShader *changeActiveShader(CShader *newShader);

	std::string m_prefix;
	std::map< std::string, std::shared_ptr<CShader> > m_shaders;
	CShader *m_activeShader;

	friend class __CShaderFactoryImplDel;
}; // class CShaderFactory


#endif // SHADERFACTORY_H