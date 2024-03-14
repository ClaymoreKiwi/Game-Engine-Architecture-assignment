#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "ShaderUtils.h"

namespace GE
{
	class Billboard
	{
	public:
		Billboard(const char* texturePath)
			:x(0.0), y(0.0), z(0.0)
		{
			texture = new Texture(texturePath);
			scaleX = scaleY = ScaleZ = 1.0f;
		}
		void BindTexture(const GLuint* PID);
		~Billboard() {};

		float getX()
		{
			return x;
		}
		float getY()
		{
			return y;
		}
		float getZ()
		{
			return z;
		}

		void setX(float newX)
		{
			x = newX;
		}
		void setY(float newY)
		{
			y = newY;
		}
		void setZ(float newZ)
		{
			z = newZ;
		}

		void setScaleX(float newX)
		{
			scaleX = newX;
		}
		void setScaleY(float newY)
		{
			scaleY = newY;
		}
		float getScaleX()
		{
			return scaleX;
		}
		float getScaleY()
		{
			return scaleY;
		}
		glm::vec3 getPos() {
			return pos;
		}

	private:
		float x = 0, y = 0, z = 0;
		float scaleX = 0, scaleY = 0, ScaleZ = 0;

		Texture* texture = nullptr;
		glm::vec3 pos = { x,y,z };
	};
}

