#include "SkyBox.h"
#include "ShaderUtils.h"
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void GE::SkyBox::Draw(Camera* camera)
{
	if (glIsEnabled(GL_DEPTH_TEST))
	{
		glDisable(GL_DEPTH_TEST);
	}
	glm::mat4 camView	 = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();

	camView[3][0] = 0.0f;
	camView[3][1] = 0.0f;
	camView[3][2] = 0.0f;

	glUseProgram(skyboxCubeProgramID);

	glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

	glUniformMatrix4fv(viewUniformID, 1, GL_FALSE, glm::value_ptr(camView));
	glUniformMatrix4fv(projectionUniformID, 1, GL_FALSE, glm::value_ptr(projection));

	glEnableVertexAttribArray(vertrxLocations);

	glVertexAttribPointer(vertrxLocations, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x));

	//bind textures and define offsets
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerID, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(CubeVertex));

	glDisableVertexAttribArray(vertrxLocations);

	//unselect the program and unbind effectively
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}

void GE::SkyBox::CreateCubeMap(std::vector<std::string> filNames)
{

	glGenTextures(1, &skyboxCubeMapName);

	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

	for (int faceNum = 0; faceNum < 6; ++faceNum)
	{
		SDL_Surface* surfaceImg = IMG_Load(filNames[faceNum].c_str());
		if (surfaceImg == nullptr)
		{
			std::cerr << "image was not loaded correctly\n";
			return;
		}

		GLenum format = surfaceImg->format->format;

		switch (format)
		{
		case SDL_PIXELFORMAT_RGBA32:
			format = GL_RGBA;
			break;
		case SDL_PIXELFORMAT_RGB24:
			format = GL_RGB;
			break;
		default:
			format = GL_RGB;
			break;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNum, 0, format, surfaceImg->w, surfaceImg->h, 0, format, GL_UNSIGNED_BYTE, surfaceImg->pixels);
		SDL_FreeSurface(surfaceImg);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GE::SkyBox::CreateCubeVBO()
{
	glGenBuffers(1, &vboSkybox);
	glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GE::SkyBox::CreateSkyBoxProgram()
{
	sb_vShader = LoadShaderFile("../NosGraphics/shaders/CubeVShader.shader");
	sb_fShader = LoadShaderFile("../NosGraphics/shaders/CubeFShader.shader");
	SetupShaderProgram(sb_vShader, sb_fShader, &skyboxCubeProgramID);

	vertrxLocations = glGetAttribLocation(skyboxCubeProgramID, "vertexPos3D");
	if (vertrxLocations == 1)
	{
		std::cerr << "Problem getting vertex3DPos" << std::endl;
	}

	viewUniformID		= glGetUniformLocation(skyboxCubeProgramID, "view");
	projectionUniformID	= glGetUniformLocation(skyboxCubeProgramID, "projection");
	samplerID			= glGetUniformLocation(skyboxCubeProgramID, "sampler");
}
