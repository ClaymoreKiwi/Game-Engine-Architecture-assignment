#pragma once
#include <glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>

#include "Camera.h"
#include "utils.h"

namespace GE
{
	class SkyBox
	{
	public:
		SkyBox(std::vector<std::string> filNames)
		{
			CreateCubeMap(filNames);
			CreateCubeVBO();
			CreateSkyBoxProgram();
		}
		~SkyBox()
		{
			glDeleteProgram(skyboxCubeProgramID);
			glDeleteBuffers(1, &vboSkybox);
			glDeleteTextures(1, &skyboxCubeMapName);
		}
		void Draw(Camera* camera);

	private:
		void CreateCubeMap(std::vector<std::string> filNames);
		void CreateCubeVBO();
		void CreateSkyBoxProgram();

	private:
		GLuint skyboxCubeMapName;
		GLuint skyboxCubeProgramID;
		
		GLint vertrxLocations;

		GLuint vboSkybox;

		GLuint viewUniformID;
		GLuint projectionUniformID;
		GLuint samplerID;

		std::string sb_vShader;
		std::string sb_fShader;
	};

	struct CubeVertex
	{
		float x, y, z;

		CubeVertex()
			:x(0.0), y(0.0), z(0.0)
		{}

		CubeVertex(float m_x, float m_y, float m_z)
			:x(m_x), y(m_y), z(m_z)
		{}
	};
	const float SIDE = 2.0f;

	static CubeVertex cube[] = {
		// Front face
		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),

		// Back face
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),

		// Left face
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, -SIDE),

		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),

		// Right face
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),

		// Top face
		CubeVertex(-SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE,  SIDE),

		// Bottom face
		CubeVertex(-SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),
	};
}

