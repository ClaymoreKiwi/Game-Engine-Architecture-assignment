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
#include "Light.h"
#include "BillboardRenderer.h"

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
		void ManipulateVerticies(glm::mat4& tranformationMat, Model* model);
		void DrawWithIndecies(GLuint vertex, GLuint index, GLuint indexNumber);
		void Fog(float density);
		//update the state of the object e.g Anims
		void Draw(Camera* cam, Model* model, Light* light);		   //Rendering method
		void Draw(Model* model, Light* light);		   //Rendering method
		void Draw(Camera* cam, Terrain* terrain, Light* light);
		void Draw(Camera* cam, SkyDome* sky, Light* light);
		void Destroy();								   //Releases object from the buffer

		//getters
		glm::vec3 getPos() const {return pos;}
		float getRotX() const {return RotX;}
		float getRotY() const {return RotY;}
		float getRotZ() const {return RotZ;}
		float getScaleX() const {return ScaleX;}
		float getScaleY() const {return ScaleY;}
		float getScaleZ() const {return ScaleZ;}

		const GLuint* getPID() {return &programID;}

		//Setters
		void setPos(float x, float y, float z)
		{
			pos.x = x;
			pos.y = y;
			pos.z = z;
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
		GLint vertexPos3DLocation = 0;   //3D attributes for the pipeline
		GLint vertexUVLocation = 0;      //recieves a UV coordiante
		GLint vertexNormal = 0;          //normals of the verticeis

		//Light uniforms
		GLint lightColour;
		GLint ambient;
		GLint lightPos;

		//position, rotation and scale variables default
		glm::vec3 pos;
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
			   samplerID = 0,
			   viewPosID = 0;

		GLuint fogColourID = 0,
			   fogDensityID = 0;

		std::string mr_vShader;
		std::string mr_fShader;

		std::unique_ptr<BillboardRenderer> billboardRend = nullptr;
	};
}

