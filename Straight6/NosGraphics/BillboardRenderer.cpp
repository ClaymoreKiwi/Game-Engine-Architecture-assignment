#include <glm/glm.hpp>
#include <iostream>
#include "BillboardRenderer.h"
#include "ShaderUtils.h"
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GE {

	// Billboard quad.  Quad is a square so use scale
	// to get correct dimensions.  Is on 0,0,0
	Vertex billboard[] = {
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),

		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f)
	};


	void BillboardRenderer::init()
	{
		bl_vShader = LoadShaderFile("../NosGraphics/shaders/Vbillboard.shader");
		bl_fShader = LoadShaderFile("../NosGraphics/shaders/Fbillboard.shader");
		SetupShaderProgram(bl_vShader, bl_fShader, &programId);

		// Now get a link to the vertexPos3D so we can link the attribute
		// to our vertices when rendering
		vertexLocation = glGetAttribLocation(programId, "vertexPos3D");

		// Check for errors
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		// Now link to the vUV attribute
		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Link the uniforms to the member fields
		transformUniformId  = glGetUniformLocation(programId, "transform");
		viewUniformId	    = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId			= glGetUniformLocation(programId, "sampler");

		// Create the vertex buffer for the quad
		GLCall(glGenBuffers(1, &vboQuad));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vboQuad));

		// Transfer vertices to graphics memory
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(billboard), billboard, GL_STATIC_DRAW));
	}

	void BillboardRenderer::draw(Billboard* b)
	{
		glEnable(GL_BLEND);
		//removes backgound colour from alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 transformationMat = glm::mat4(1.0f);
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 projectionMat = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);
		// Select the program into the rendering context
		GLCall(glUseProgram(programId));

		// Set the uniforms in the shader
		GLCall(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
		GLCall(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
		GLCall(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

		b->BindTexture(&programId);
		SetupMatricies(vboQuad, vertexLocation, vertexUVLocation, 6);

		glDisable(GL_BLEND);
	}

	void BillboardRenderer::draw(Billboard* b, Camera* cam)
	{
		glEnable(GL_BLEND);
		//removes backgound colour from alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Calculate the transformation matrix for the object.  Start with the identity matrix
		// No rotation as going to look down the z axis
		glm::mat4 transformationMat = glm::mat4(1.0f);
		
		// Position in world
		transformationMat	= glm::translate(transformationMat, glm::vec3(b->getX(), b->getY(), b->getZ()));
		glm::vec3 direction = (cam->getPos() - b->getPos());
		float angle			= atan2(direction.x, direction.z);
		transformationMat	= glm::rotate(transformationMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		transformationMat = glm::scale(transformationMat, glm::vec3(b->getScaleX(), b->getScaleY(), 0.0f));
		// Render object in the same way as a regular model
		// Get the view and projection matrices
		glm::mat4 viewMat		= cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context
		GLCall(glUseProgram(programId));

		// Set the uniforms in the shader
		GLCall(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
		GLCall(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
		GLCall(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

		b->BindTexture(&programId);
		SetupMatricies(vboQuad,vertexLocation,vertexUVLocation, 6);

		glDisable(GL_BLEND);
	}

}