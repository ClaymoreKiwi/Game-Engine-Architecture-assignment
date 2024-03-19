#include "ShaderUtils.h"
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
		GLCall(glShaderSource(Shader, 1, &src, NULL));

		//compile shader
		GLCall(glCompileShader(Shader));

		//get the compile status of the program
		GLCall(glGetShaderiv(Shader, GL_COMPILE_STATUS, &isShaderCompiledOK));
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
		GLCall(glAttachShader(*programID, vShader));
		GLCall(glAttachShader(*programID, fShader));

		//link program to an exe - exists in graphics meemory
		GLCall(glLinkProgram(*programID));

		//check any linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(*programID, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE)
		{
			std::cerr << "Failed to link program" << std::endl;
		}
	}
	void SetupMatricies(const GLuint& vbo, GLint& vertexLoc, GLint& vertexUVLoc, const GLuint& vertexCount)
	{
		//select the vertex buffer into context
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

		//enable the attributes that will be used for the buffer object
		//location x,y,z and its appropriate jumps
		GLCall(glEnableVertexAttribArray(vertexLoc));
		GLCall(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)));

		//enable the attributes to pass in colours to the buffer object
		//location u,v and its appropriate jump
		GLCall(glEnableVertexAttribArray(vertexUVLoc));
		GLCall(glVertexAttribPointer(vertexUVLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u)));

		//draw triangles base off its verticies
		GLCall(glDrawArrays(GL_TRIANGLES, 0, vertexCount));
		//unselect the attribute
		GLCall(glDisableVertexArrayAttrib(GL_ARRAY_BUFFER, 0));
		GLCall(glDisableVertexAttribArray(vertexLoc));
		GLCall(glDisableVertexAttribArray(vertexUVLoc));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		//unselect the program from this context
		GLCall(glUseProgram(0));
	}
}