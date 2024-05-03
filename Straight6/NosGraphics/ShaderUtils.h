#pragma once
#include <glew.h>
#include <string>

#include "utils.h"
namespace GE 
{
	/// <summary>
	/// Call Setup Shaders to complete all shader and program setup
	/// </summary>
	/// <param name="vShaderPath"></param>
	/// <param name="fShaderPath"></param>
	/// <param name="ProgramID"></param>
	void SetupShaderProgram(std::string& vShaderPath, std::string& fShaderPath, GLuint* programID);
	GLuint ShaderSetup(GLint type, const std::string& shaderPath);
	void ProgramSetup(const GLuint& vShader, const GLuint& fShader, GLuint* programID);
	void DrawWithVerticies(const GLuint& vbo, GLint& vertexLoc, GLint& vertexUVLoc, const GLuint& vertexCount);
	void DrawWithVerticies(const GLuint& vbo, GLint& vertexLoc, GLint& vertexUVLoc, GLint& vertexNorm, const GLuint& vertexCount);
	void setupVertexInfo(const GLuint& vbo, const GLint& vertexLoc, const GLint& vertexUVLoc, const GLint& vertexNorm);
}
