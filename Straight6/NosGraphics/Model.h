#pragma once
#include <random>
#include "utils.h"
#include "Billboard.h"
#include "Texture.h"

namespace GE
{
	class Model
	{
	public:
		Model(const GLuint* programID, const char* modelPath, const char* texturePath, const char* billboardTexture, glm::vec3& pos);
		~Model()
		{
			delete billboard;
			glDeleteBuffers(1, &vbo);
		}
		void BindTexture();
		bool LoadFromFile(const char* fileName, const char* shaderTex);
		bool LoadFromFileV2(const char* fileName);
		inline const GLuint getVertices() {return vbo;}
		inline const GLuint getInstancedVbo() {return instanceVbo;}
		inline const int getNumOfVertices() {return numVertices;}
		GE::Billboard* getBillboard() {return billboard;}

		//getters
		glm::vec3 getPos() { return pos; }
		float getRotX() const { return RotX; }
		float getRotY() const { return RotY; }
		float getRotZ() const { return RotZ; }
		float getScaleX() const { return ScaleX; }
		float getScaleY() const { return ScaleY; }
		float getScaleZ() const { return ScaleZ; }

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
		inline bool HasValidBillboard()
		{
			return hasValidBillboard;
		}
	private:

		GLuint vbo = 0,
			   instanceVbo = 0;

		Billboard* billboard = nullptr;
		std::unique_ptr<GE::Texture> texture = nullptr;
		const GLuint* PIDref = nullptr;

		int numVertices = 0;
		glm::vec3 pos = {0,0,0};
		//Rotation
		float RotX = 0.0f,
			  RotY = 0.0f,
			  RotZ = 0.0f;
		//Scale
		float ScaleX = 0.0f,
			  ScaleY = 0.0f,
			  ScaleZ = 0.0f;

		bool hasValidBillboard = true;
	};
}

