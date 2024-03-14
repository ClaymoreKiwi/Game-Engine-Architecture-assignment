#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

//this function is a helper function that allows us to see
//errors when the shaders do not compile ok.
inline void DisplayShaderCompilerError(GLuint shaderId)
{
	//Get length of message
	GLint msgLength = 0;

	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLength);

	//only display a message when the legth is > 1
	if (msgLength > 1)
	{
		std::unique_ptr<GLchar[]> msg(new GLchar[msgLength + 1]);			//better use of memory management with unique_ptr (avoid new and delete[]
		glGetShaderInfoLog(shaderId, msgLength, NULL, msg.get());			//gets error from log and put them in message buffer
		std::cerr << "Error compiling shader: " << msg.get() << std::endl;  //finally display error
	}
	//unique_ptr self managed and out of scope here
}

inline std::string LoadShaderFile(const std::string ShaderPath)
{
	std::ifstream file(ShaderPath);
	if (!file.is_open()) // Check if file is open
	{
		std::cerr << "Error opening file: " << ShaderPath << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close(); // Close the file

	return buffer.str();
}

//Vertex info as utility for every class that needs it
struct Vertex
{
	//pos
	float x, y, z = 0;
	//UV Textures
	float u,v = 0;
	//Constructors
	//specific vertex placement
	Vertex(float m_x, float m_y, float m_z, float m_u, float m_v)
		:x(m_x), y(m_y), z(m_z), u(m_u), v(m_v)
	{}
	//vertex placement origin
	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
		u = v = 0;
	}
};

