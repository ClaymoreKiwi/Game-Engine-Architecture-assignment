#include "FPSCamera.h"


GE::FPSCamera::FPSCamera(glm::vec3 target, glm::vec3 cam_pos, glm::vec3 cam_upDir, float fov, float ar, float near, float far)
	:Camera(target, cam_pos, cam_upDir, fov, ar, near, far)
{
	updateCamMatrices();

	SDL_Window* window = SDL_GL_GetCurrentWindow();

	int w, h;

	SDL_GetWindowSize(window, &w, &h);

	SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);
}

void GE::FPSCamera::updateCamMatrices()
{
	viewMat = glm::lookAt(getPos(), getPos() + getTarget(), getUpDir());
	projectionMat = glm::perspective(glm::radians(getFovY()), getAspectRatio(), getNearPlane(), getFarPlane());
}
