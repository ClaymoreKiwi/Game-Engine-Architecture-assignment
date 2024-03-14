#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

namespace GE {
	class Camera {
	protected:
		Camera(glm::vec3 target, glm::vec3 cam_pos, glm::vec3 cam_upDir, float fov, float ar, float near, float far);

		virtual ~Camera() {}
	public:

		// Accessor methods
		// Return the position of the camera in terms of x, y, z
		const float getPosX() {
			return pos.x;
		}
		const float getPosY() {
			return pos.y;
		}
		const float getPosZ() {
			return pos.z;
		}
		const float getNearPlane()
		{
			return nearClip;
		}
		const float getFarPlane()
		{
			return farClip;
		}
		const float getPitch()
		{
			return pitch;
		}
		const float getYaw()
		{
			return yaw;
		}
		const float getFovY()
		{
			return fovy;
		}
		const float getAspectRatio()
		{
			return aspectR;
		}

		// Target or what location the camera is looking at
		glm::vec3 getTarget() const {
			return target;
		}
		// Up direction
		glm::vec3 getUpDir() const {
			return up;
		}
		// Return the camera's view matrix.  Used by draw
		// method to send view matrix to vertex shader
		glm::mat4 getViewMatrix() const {
			return viewMat;
		}
		// Return camera's projection matrix.
		glm::mat4 getProjectionMatrix() const {
			return projectionMat;
		}
		//get position of the camera
		glm::vec3 getPos() {
			return pos;
		}



		// Mutator methods
		// Set position
		void setPosX(float newX) {
			pos = glm::vec3(newX, pos.y, pos.z);
			updateCamMatrices();
		}
		void setPosY(float newY) {
			pos = glm::vec3(pos.x, newY, pos.z);
			updateCamMatrices();
		}
		void setPosZ(float newZ) {
			pos = glm::vec3(pos.x, pos.y, newZ);
			updateCamMatrices();
		}

		// Set position for all axes in one method
		void setPos(glm::vec3 newPos) {
			pos = newPos;
			updateCamMatrices();
		}
		// Set new target
		void setTarget(glm::vec3 newTarget) {
			target = newTarget;
			updateCamMatrices();
		}
		// Set the up direction
		void setUpDir(glm::vec3 newUp) {
			up = newUp;
			updateCamMatrices();
		}
		//set angle of rotatoion on X
		void setPitch(float newPitch)
		{
			pitch = newPitch;
			if (pitch > 70.0f)
				pitch = 70.0f;
			if(pitch < -70.0f)
				pitch = -70.0f;
		}
		//set angle of rotation on Y
		void setYaw(float newYaw)
		{
			yaw = newYaw;
		}
		// Set the fov
		void setFov(float newFov) {
			fovy = newFov;
			updateCamMatrices();
		}
		// Set the aspect ratio.
		void setAspectRatio(float newAR) {
			aspectR = newAR;
			updateCamMatrices();
		}
		// Set clipping planes
		void setNearClip(float newNearClip) {
			nearClip = newNearClip;
			updateCamMatrices();
		}
		//set far plane
		void setFarClip(float newFarClip) {
			farClip = newFarClip;
			updateCamMatrices();
		}
		// Update the camera matrices done in response to a member variable variable being updated
		virtual void updateCamMatrices() {
			viewMat = glm::lookAt(pos, target, up);
			projectionMat = glm::perspective(glm::radians(fovy), aspectR, nearClip, farClip);
		}


	private:
		// Member variables
		// Camera view variables
		glm::vec3 pos;
		glm::vec3 target;
		glm::vec3 up;

		// Camera projection variables
		float fovy;
		float aspectR;
		float nearClip;
		float farClip;

		//cam rotational values
		float pitch = 0.0f;
		float yaw = -90.0f; //orent along the z axis

	protected:
		// View and projection matrices
		glm::mat4 viewMat;
		glm::mat4 projectionMat;
	};

}
