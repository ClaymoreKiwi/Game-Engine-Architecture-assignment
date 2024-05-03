#include "InstancedRenderer.h"
#include "ShaderUtils.h"
#include "utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	InstancedRenderer::InstancedRenderer() 
	{
		init();
	}

	void InstancedRenderer::init()
	{
		std::string v_shader_source = LoadShaderFile("../NosGraphics/shaders/instanceV.shader");
		std::string f_shader_source = LoadShaderFile("../NosGraphics/shaders/instanceF.shader");
		SetupShaderProgram(v_shader_source, f_shader_source, &programId);

		vertexLocation = glGetAttribLocation(programId, "vertexPos3D");
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		vertexUVLocation = glGetAttribLocation(programId, "vUV");
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		instanceMatLocation = glGetAttribLocation(programId, "instanceTransform");
		if (instanceMatLocation == -1) {
			std::cerr << "Problem getting instanceTransform" << std::endl;
		}

		vertexNormal = glGetAttribLocation(programId, "vertexNormal");
		if (vertexNormal == -1)
		{
			std::cerr << "Problem getting vertex Normal" << std::endl;
		}
		// Link the uniforms to the member fields
		viewUniformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");
		viewPosID = glGetUniformLocation(programId, "viewPos");

		//light info
		lightColour = glGetUniformLocation(programId, "lightColour");
		lightPos = glGetUniformLocation(programId, "lightPos");
		ambient = glGetUniformLocation(programId, "lightPosambient");
	}

	void InstancedRenderer::Fog(float density)
	{
		fogColourID = glGetUniformLocation(programId, "fog_colour");
		fogDensityID = glGetUniformLocation(programId, "fog_density");

		GLCall(glUniform1f(fogDensityID, density));
		glm::vec3 fog_colour = glm::vec3(0.5f, 0.5f, 0.5f);
		GLCall(glUniform3fv(fogColourID, 1, glm::value_ptr(fog_colour)));
	}

	void InstancedRenderer::createInstances(int instances)
	{
		std::vector<glm::mat4> instanceMat;
		instanceMat.resize(instances, glm::mat4(1));
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> disX(250.0f, 450.0f); // for x
		std::uniform_real_distribution<float> disY(10.0f, 20.0f);    // for y
		std::uniform_real_distribution<float> disZ(100.0f, 850.0f); // for z

		for (int i = 0; i < instances; ++i) {
			float randX = disX(gen);
			float randY = disY(gen);
			float randZ = disZ(gen);
			float rotAngle = (rand() % 360);
			instanceMat[i] = glm::scale(instanceMat[i], glm::vec3(0.5, 0.5, 0.5));
			instanceMat[i] = glm::translate(instanceMat[i], glm::vec3(randX, randY, randZ));
			instanceMat[i] = glm::rotate(instanceMat[i], rotAngle, glm::vec3(0, 1, 0));
		}

		numInstances = instanceMat.size();

		GLCall(glGenBuffers(1, &instanceMatrixBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceMat.size(), instanceMat.data(), GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	// Draw objects using instancing
	void InstancedRenderer::DrawInstanced(Camera* cam, Model* m, Light* light) {

		glEnable(GL_CULL_FACE);

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context
		glUseProgram(programId);

		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		GLCall(glUniform3f(lightColour, light->getColour().x, light->getColour().y, light->getColour().z));
		GLCall(glUniform3f(ambient, light->getAmbient().x, light->getAmbient().y, light->getAmbient().z));
		GLCall(glUniform3f(lightPos, light->getPos().x, light->getPos().y, light->getPos().z));

		setupVertexInfo(m->getVertices(), vertexLocation, vertexUVLocation, vertexNormal);

		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);

		for (int i = 0; i < 4; ++i) {
			GLCall(glEnableVertexAttribArray(instanceMatLocation + i));
			GLCall(glVertexAttribPointer(instanceMatLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4))));
			GLCall(glVertexAttribDivisor(instanceMatLocation + i, 1));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m->BindTexture();
		Fog(0.0060f);
		glDrawArraysInstanced(GL_TRIANGLES, 0, m->getNumOfVertices(), numInstances);

		for (int i = 0; i < 4; ++i) {
			GLCall(glVertexAttribDivisor(instanceMatLocation + i, 0));
		}

		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		for (int i = 0; i < 4; ++i) {
			GLCall(glDisableVertexAttribArray(instanceMatLocation + i));
		}

		// Unselect the program from the context
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	void InstancedRenderer::DrawInstanced(Model* m, Light* light)
	{
		glEnable(GL_FRONT);
		//use program created from init
		GLCall(glUseProgram(light->GetShadowProgram()));

		//GLCall(glUniformMatrix4fv(glGetUniformLocation(light->GetShadowProgram(), "model"), 1, GL_FALSE, value_ptr(instanceMat)));
		GLCall(glUniformMatrix4fv(light->GetGetLightLoc(), 1, GL_FALSE, glm::value_ptr(light->getProjectionMatrix())));
		
		glDrawArraysInstanced(GL_TRIANGLES, 0, m->getNumOfVertices(), numInstances); 
		glDisable(GL_FRONT);
	}

	// Release objects allocated for program and vertex buffer object
	void InstancedRenderer::destroy()
	{
		glDeleteBuffers(1, &instanceMatrixBuffer);
		glDeleteProgram(programId);
	}

}


