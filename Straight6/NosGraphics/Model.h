#pragma once
#include "utils.h"
#include "Texture.h"

namespace GE
{
	class Model
	{
	public:
		Model() {};
		Model(const GLuint* programID, const char* modelPath, const char* texturePath);
		~Model()
		{
			glDeleteBuffers(1, &vbo);
		}
		void BindTexture();
		bool LoadFromFile(const char* fileName, const char* shaderTex);

		inline const GLuint getVertices()
		{
			return vbo;
		}

		inline const int getNumOfVertices()
		{
			return numVertices;
		}
	private:
		GLuint vbo = 0;
		GE::Texture* texture = nullptr;
		const GLuint* PIDref = nullptr;
		int numVertices = 0;
	};
}

