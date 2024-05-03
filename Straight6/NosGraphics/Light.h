#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include "Camera.h"
#include "FrameBuffer.h"
#include "ShaderUtils.h"
#include <iostream>

namespace GE
{
	class Light
	{
	public:
		Light(glm::vec3 target, glm::vec3 light_pos, Camera* camera);
		void SetLight(glm::vec3& colour, glm::vec3& ambient);
		void MakeShadowMap();
		~Light() {}

		// Accessor methods
		const float getNearPlane() {return nearClip;}
		const float getFarPlane() {return farClip;}

		glm::vec3 getTarget() const {return target;}
		glm::vec3 getUpDir() const {return up;}
		glm::mat4 getProjectionMatrix() const {return lightProjection;}
		glm::vec3 getPos() const {return pos;}
		glm::vec3 getAmbient() const { return ambientLight; }
		glm::vec3 getColour() const { return lightColour; }
		// Mutator methods

		// Update the camera matrices done in response to a member variable variable being updated
		inline void updateLightMatrices() {
			glm::mat4 viewMat = glm::lookAt(pos, target, up);
			glm::mat4 projectionMat = glm::ortho(left,right,bottom,top,nearClip,farClip);
			lightProjection = projectionMat * viewMat;
		}

		int GetShadowWidth(){return shadowWidth;}
		int GetShadowHeight() {return shadowHeight;}
		GLuint GetFBO() {return shadowMapFBO;}
		GLuint GetShadowMap() {return shadowMap;}
		GLuint GetShadowProgram() { return shadowProgramID; }
		GLuint GetGetLightLoc() { return lightProjectionLoc; }
	private:
		glm::vec3 pos;
		glm::vec3 target;
		glm::vec3 up;
		glm::vec3 lightColour;
		glm::vec3 ambientLight;

		// Camera projection variables
		float top = 100;
		float right = 100;
		float bottom = -100;
		float left = -100;
		float nearClip;
		float farClip;

		int shadowWidth, shadowHeight;

		glm::mat4 lightProjection;

		GLuint shadowProgramID = 0;
		GLuint lightProjectionLoc = 0;
		GLuint shadowMapFBO;
		GLuint shadowMap;
	};
}

