#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

//error handling debug break for compiler speecific usage
#ifdef _MSC_VER // Microsoft Visual Studio compiler
#define DEBUGBREAK __debugbreak();
#elif defined(__GNUC__) // GCC, Clang
#define DEBUGBREAK __builtin_trap();
#else
#error "Unknown compiler! Implement debug break for this compiler."
#endif

//https://www.youtube.com/watch?v=FBbPWSOQ0-w&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=10
//error handling for 
#define ASSERT(x) if (!(x)) DEBUGBREAK
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = GL_NO_ERROR)
	{
		std::cerr << "[OpenGL Error] (" << error << ")" << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

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
	float x = 0, y = 0, z = 0;
	//UV Textures
	float u = 0,v = 0;
	//normals
	float nx = 0, ny = 0, nz = 0;
	//Constructors
	//specific vertex placement
	Vertex(float m_x, float m_y, float m_z, float m_u, float m_v)
		:x(m_x), y(m_y), z(m_z), u(m_u), v(m_v)
	{
		nx = ny = nz = 0.0f;
	}
	Vertex(float m_x, float m_y, float m_z, float m_u, float m_v, float m_nx, float m_ny, float m_nz)
		:x(m_x), y(m_y), z(m_z), u(m_u), v(m_v), nx(m_nx), ny(m_ny), nz(m_nz)
	{}
	//vertex placement origin
	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
		u = v = 0;
		nx =  ny = nz = 0;
	}
};

