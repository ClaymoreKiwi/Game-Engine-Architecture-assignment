#pragma once
#include <glew.h>
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Model.h"

namespace GE {

	class InstancedRenderer
	{
	public:
		InstancedRenderer();
		~InstancedRenderer() {};

		void init();
		void createInstances(int instances);
		void DrawInstanced(Camera* cam, Model* m, Light* light);
		void DrawInstanced(Model* m, Light* light);
		void Fog(float density);
		void destroy();

	private:
		// Member fields
		// This member stores the program object that contains the shaders
		GLuint programId = 0;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation = 0;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		GLint vertexUVLocation = 0;

		// Link instance matrix attribute
		GLint instanceMatLocation = 0;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId = 0;
		GLuint viewUniformId = 0;
		GLuint projectionUniformId = 0;
		GLuint samplerId = 0;
		GLint vertexNormal = 0;

		GLint lightColour = 0;
		GLint ambient = 0;
		GLint lightPos = 0;

		GLuint fogColourID = 0,
			   viewPosID = 0,
			   fogDensityID = 0;

		int numInstances = 0;
		GLuint instanceMatrixBuffer = 0;

	};
}

