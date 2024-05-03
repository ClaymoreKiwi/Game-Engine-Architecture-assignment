#include "Light.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GE::Light::Light(glm::vec3 targetP, glm::vec3 light_pos, Camera* camera)
	:target(targetP), pos(light_pos)
{
	shadowWidth = 1024;
	shadowHeight = 1024;
	up = glm::vec3(0,1,0);
	nearClip = camera->getNearPlane();
	farClip = 500;
	updateLightMatrices();

	std::string v_shader_source = LoadShaderFile("../NosGraphics/shaders/shadowVshader.shader");
	std::string f_shader_source = LoadShaderFile("../NosGraphics/shaders/shadowFshader.shader");
	SetupShaderProgram(v_shader_source, f_shader_source, &shadowProgramID);

	lightProjectionLoc = glGetUniformLocation(shadowProgramID, "lightProjection");
	if (lightProjectionLoc == -1)
	{
		std::cerr << "Problem getting lightProjection" << std::endl;
	}
}

void GE::Light::SetLight(glm::vec3& colour, glm::vec3& ambient)
{
	lightColour = colour;
	ambientLight = ambient;
}

void GE::Light::MakeShadowMap()
{
	//shadow buffer
	glGenFramebuffers(1, &shadowMapFBO);
	//shadow dimentions & texture setup
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	//use depth components
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//set texture to scene
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	//use clamp values to make grayscale
	float clampColour[] = {1.0,1.0,1.0,1.0};
	GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColour));
	//bind and use the buffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO));
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
