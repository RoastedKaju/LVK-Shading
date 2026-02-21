#pragma once

#include <string>
#include <lvk/LVK.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class ICamera
{
public:
	ICamera() = default;
	virtual ~ICamera() = default;

	virtual glm::mat4 getViewMatrix() const = 0;
	virtual glm::mat4 getProjectionMatrix() const = 0;
	virtual glm::vec3 getCameraPosition() const = 0;

	virtual void setAspectRatio(float aspectRatio) = 0;

};

class Camera : public ICamera
{
public:
	Camera() { updateVectors(); }
	Camera(const glm::vec3& position, const glm::vec3& target)
	{
		position_ = position;
		const glm::vec3 direction = glm::normalize(target - position_);

		pitch_ = glm::degrees(asin(direction.y));
		yaw_ = glm::degrees(atan2(direction.z, direction.x));

		updateVectors();
	}

	virtual glm::mat4 getViewMatrix() const override { return glm::lookAt(position_, position_ + front_, up_); };
	virtual glm::mat4 getProjectionMatrix() const override { return glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_); };
	virtual glm::vec3 getCameraPosition() const override { return position_; }

	virtual void setAspectRatio(float aspectRatio) override { aspectRatio_ = aspectRatio; }

protected:
	void updateVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		front.y = sin(glm::radians(pitch_));
		front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

		front_ = glm::normalize(front);
		right_ = glm::normalize(glm::cross(front_, worldUp_));
		up_ = glm::normalize(glm::cross(right_, front_));
	}

	float fov_ = 45.0f;
	float aspectRatio_ = 1.7777778f;
	float nearPlane_ = 0.1f;
	float farPlane_ = 1000.0f;

	// Transform
	glm::vec3 position_ = glm::vec3(0.0f);
	glm::vec3 front_ = glm::vec3(0.0f, 0.0f, -1.0f);;
	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right_ = glm::vec3(0.0f);
	glm::vec3 worldUp_ = glm::vec3(0.0f, 1.0f, 0.0f);

	// angles
	float yaw_ = 0.0f;
	float pitch_ = 0.0f;
};

class FreeCamera : public Camera
{
public:
	FreeCamera(GLFWwindow* window, const glm::vec3& position, const glm::vec3& target) : Camera(position, target)
	{
		yawDesired_ = yaw_;
		pitchDesired_ = pitch_;

		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
				{
					// retrieve your camera pointer stored in the window
					FreeCamera* camera = static_cast<FreeCamera*>(glfwGetWindowUserPointer(window));
					camera->cursorVisible_ = !camera->cursorVisible_;
					glfwSetInputMode(window, GLFW_CURSOR, camera->cursorVisible_ ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
				}
			});
	}

	void handleInput(GLFWwindow* window, float deltaTime)
	{
		// WASD
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			position_ += front_ * moveSpeed_ * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			position_ -= front_ * moveSpeed_ * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			position_ -= right_ * moveSpeed_ * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			position_ += right_ * moveSpeed_ * deltaTime;
		// Up and Down
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			position_ += worldUp_ * moveSpeed_ * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			position_ -= worldUp_ * moveSpeed_ * deltaTime;
		// Mouse
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (firstMouse_)
		{
			lastMouseX_ = (float)mouseX;
			lastMouseY_ = (float)mouseY;
			firstMouse_ = false;
		}

		float xOffset = (float)(mouseX - lastMouseX_) * sensitivity_;
		float yOffset = (float)(lastMouseY_ - mouseY) * sensitivity_; // inverted Y

		lastMouseX_ = (float)mouseX;
		lastMouseY_ = (float)mouseY;

		if (!cursorVisible_)
		{
			yawDesired_ += xOffset;
			pitchDesired_ += yOffset;
		}
		pitchDesired_ = glm::clamp(pitchDesired_, -89.0f, 89.0f); // prevent flipping

		// Smoothly interpolate current toward desired:
		yaw_ += (yawDesired_ - yaw_) * damping_ * deltaTime;
		pitch_ += (pitchDesired_ - pitch_) * damping_ * deltaTime;

		updateVectors();
	}

protected:
	float moveSpeed_ = 0.5f;
	float sensitivity_ = 0.35f;
	bool firstMouse_ = true;
	float lastMouseX_ = 0.0f;
	float lastMouseY_ = 0.0f;
	bool cursorVisible_ = true;
	float yawDesired_ = 0.0f;
	float pitchDesired_ = 0.0f;
	float damping_ = 15.0f;
};