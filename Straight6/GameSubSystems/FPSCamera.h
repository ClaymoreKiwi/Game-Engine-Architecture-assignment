#pragma once
#include "Camera.h"
#include <iostream>
namespace GE
{
	class FPSCamera : public Camera
	{
	public:

		FPSCamera(glm::vec3 target, glm::vec3 cam_pos, glm::vec3 cam_upDir, float fov, float ar, float near, float far);

		void updateCamMatrices() override;
	};
}

