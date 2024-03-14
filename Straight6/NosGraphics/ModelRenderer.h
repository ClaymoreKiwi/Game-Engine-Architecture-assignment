#pragma once
#include <glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "Model.h"
#include "Terrain.h"
#include "Texture.h"
#include "SkyDome.h"

namespace GE
{
	class ModelRenderer
	{
	public:
		ModelRenderer()
			:ScaleX(0.5f), ScaleY(0.5f), ScaleZ(0.5f)
		{
			Init();
		}
		virtual ~ModelRenderer() {} //does nothing

		void Init();								   //create shaders and vertex buffer
		void Update();
		void ManipulateVerticies(glm::mat4& tranformationMat);
		//update the state of the object e.g Anims
		void Draw(Camera* cam, Model* model);		   //Rendering method
		void Draw(Camera* cam, Terrain* terrain);
		void Draw(Camera* cam, SkyDome* sky);
		void Destroy();								   //Releases object from the buffer

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

		const GLuint* getPID() //get program ID
		{
			return &programID;
		}

		//Setters
		void setPos(float x, float y, float z)
		{
			posX = x;
			posY = y;
			posZ = z;
		}

		void setRotation(float x, float y, float z)
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
		GLuint vboTriangle = 0;			 //store triange vertex buffer containing vertices - transfered to graphics memory

		GLint vertexPos3DLocation = 0;   //3D attributes for the pipeline
		GLint vertexUVLocation = 0;      //recieves a UV coordiante

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
			   projectionUniformID = 0,
			   samplerID = 0;

		std::string mr_vShader;
		std::string mr_fShader;

		Texture* texture = nullptr;

	};
}

