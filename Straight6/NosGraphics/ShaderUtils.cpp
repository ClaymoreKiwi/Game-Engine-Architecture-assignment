#include "ShaderUtils.h"
#include "utils.h"
#include <iostream>

namespace GE {

	void SetupShaderProgram(std::string& vShaderPath, std::string& fShaderPath, GLuint* programID)
	{
		GLuint vs = ShaderSetup(GL_VERTEX_SHADER, vShaderPath);
		GLuint fs = ShaderSetup(GL_FRAGMENT_SHADER, fShaderPath);
		ProgramSetup(vs, fs, programID);
	}

	GLuint ShaderSetup(GLint type, const std::string& shaderPath)
	{
		GLuint Shader = glCreateShader(type);
		const char* src = shaderPath.c_str();
		GLint isShaderCompiledOK = GL_FALSE;
		//copy the source to openGL ready to compile
		glShaderSource(Shader, 1, &src, NULL);

		//compile shader
		glCompileShader(Shader);

		//get the compile status of the program
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE)
		{
			std::cerr << "Unable to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
			DisplayShaderCompilerError(Shader);
			return 0;
		}
		return Shader;
	}

	void ProgramSetup(const GLuint& vShader, const GLuint& fShader, GLuint* programID)
	{
		*programID = glCreateProgram();
		//attached both shaders
		glAttachShader(*programID, vShader);
		glAttachShader(*programID, fShader);

		//link program to an exe - exists in graphics meemory
		glLinkProgram(*programID);

		//check any linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(*programID, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE)
		{
			std::cerr << "Failed to link program" << std::endl;
		}
	}
}