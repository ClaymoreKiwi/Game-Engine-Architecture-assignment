#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "Texture.h"
#include "ShaderUtils.h"

namespace GE
{
	class Billboard
	{
	public:
		Billboard(const char* texturePath, glm::vec3 pos_)
			:pos(pos_)
		{
			texture = std::make_unique<Texture>(texturePath);
			scaleX = scaleY = ScaleZ = 1.0f;
		}
		Billboard(Texture* texture)
			:texture(texture)
		{
			scaleX = scaleY = ScaleZ = 1.0f;
		}
		void BindTexture(const GLuint* PID);
		~Billboard() {};

		float getX()
		{
			return pos.x;
		}
		float getY()
		{
			return pos.y;
		}
		float getZ()
		{
			return pos.z;
		}

		void setX(float newX)
		{
			pos.x = newX;
		}
		void setY(float newY)
		{
			pos.y = newY;
		}
		void setZ(float newZ)
		{
			pos.z = newZ;
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
		void setPos(float x_, float y_, float z_)
		{
			pos.x = x_;
			pos.y = y_;
			pos.z = z_;
		}

	private:
		float scaleX = 0, scaleY = 0, ScaleZ = 0;

		std::unique_ptr<Texture> texture = nullptr;
		glm::vec3 pos = {};
	};
}

