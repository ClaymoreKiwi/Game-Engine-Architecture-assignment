#include "Camera.h"
namespace GE
{
	Camera::Camera(glm::vec3 target, glm::vec3 cam_pos, glm::vec3 cam_upDir, float fov, float ar, float near, float far)
		:target(target), pos(cam_pos), up(cam_upDir), fovy(fov), aspectR(ar), nearClip(near), farClip(far) // Initialise the camera variables to the values passed on the parameters
	{
		updateCamMatrices();
	}
}