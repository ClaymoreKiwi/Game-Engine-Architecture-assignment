#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Billboard.h"

namespace GE {
	class BillboardRenderer
	{
	public:
		BillboardRenderer() 
		{
			init();
		};
		BillboardRenderer(bool isInstanced_)
			:isInstanced(isInstanced_)
		{
			init();
		};
		~BillboardRenderer() {
			glDeleteBuffers(1, &vboQuad);
		}

		void init();
		void draw(Billboard*, Camera*);
		void drawAsParticleV2(Billboard* billboard, Camera* cam, const GLuint buffer);
		void setPos(const glm::vec3& pos_)
		{
			pos = pos_;
		}

	private:
		glm::vec3 pos;
		// Member fields
		// This member stores the program object that contains the shaders
		GLuint programId = 0;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation = 0;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		GLint vertexUVLocation = 0;

		//used to adjust the colour and opacity of particles
		//default colour will be 1 for all non particles
		GLint AdjustColourID = 0;

		// This member stores the triangle vertex buffer object containing the vertices
		// transferred from this code to the graphics memory
		GLuint vboQuad = 0;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId = 0;
		GLuint viewUniformId = 0;
		GLuint projectionUniformId = 0;
		GLuint samplerId = 0;

		//instance
		GLuint particlePosLoc = 0;
		GLuint particleFadeLoc = 0;

		//shaders
		std::string bl_vShader = "";
		std::string bl_fShader = "";

		bool isInstanced = false;
	};
}

