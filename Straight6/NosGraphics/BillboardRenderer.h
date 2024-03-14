#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Billboard.h"

namespace GE {
	class BillboardRenderer
	{
	public:
		BillboardRenderer() {};

		~BillboardRenderer() {
			glDeleteBuffers(1, &vboQuad);
		}

		void init();

		void draw(Billboard*, Camera*);

		void setPos(float x, float y, float z)
		{
			posX = x;
			posY = y;
			posZ = z;
		}

	private:
		float posX = 0.0f,
			  posY = 0.0f,
			  posZ = 0.0f;
		// Member fields
		// This member stores the program object that contains the shaders
		GLuint programId = 0;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation = 0;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		GLint vertexUVLocation = 0;

		// This member stores the triangle vertex buffer object containing the vertices
		// transferred from this code to the graphics memory
		GLuint vboQuad = 0;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId = 0;
		GLuint viewUniformId = 0;
		GLuint projectionUniformId = 0;
		GLuint samplerId = 0;

		std::string bl_vShader = "";
		std::string bl_fShader = "";
	};
}

