#pragma once
#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>

#include "Camera.h"

namespace GE
{
	class CubeRenderer
	{
	public:
		CubeRenderer() {}		    //does nothing
		virtual ~CubeRenderer() {} //does nothing

		void Init();				 //create shaders and vertex buffer
		void Update();				 //update the state of the object e.g Anims
		void Draw(Camera* cam);		 //Rendering method
		void Destroy();				 //Releases object from the buffer

		//getters
		const float getPosX() {
			return posX;
		}

		const float getPosY() {
			return posY;
		}

		const float getPosZ() {
			return posZ;
		}

		const float getRotX() {
			return RotX;
		}

		const float getRotY() {
			return RotY;
		}

		const float getRotZ() {
			return RotZ;
		}

		const float getScaleX() {
			return ScaleX;
		}

		const float getScaleY() {
			return ScaleY;
		}

		const float getScaleZ() {
			return ScaleZ;
		}

		//Setters
		void setPos(float x, float y, float z)
		{
			posX = x;
			posY = y;
			posZ = z;
		}

		void setrotation(float x, float y, float z)
		{
			RotX = x;
			RotY = y;
			RotZ = z;
		}

		void setScale(float x, float y, float z)
		{
			ScaleX = x;
			ScaleY = y;
			ScaleZ = z;
		}

	private:
		GLuint programID = 0;			 // program object that contains the shaders
		GLuint vboCube = 0;			 //store triange vertex buffer containing vertices - transfered to graphics memory

		GLint vertexPos3DLocation = 0;   //3D attributes for the pipeline
		GLint vertexUVLocation = 0;

		//position, rotation and scale variables default
		float posX = 0.0f,
			posY = 0.0f,
			posZ = 0.0f;
		//Rotation
		float RotX = 0.0f,
			RotY = 0.0f,
			RotZ = 0.0f;
		//Scale
		float ScaleX = 0.0f,
			ScaleY = 0.0f,
			ScaleZ = 0.0f;

		//IDs
		GLuint transformUniformID = 0,
			viewUniformID = 0,
			projectionUniformID = 0;
	};
}

