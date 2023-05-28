#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <glm/glm.hpp>

using glm::vec3;

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.02f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

#define DTOR 0.0174532925    
#define RTOD 57.2957795 

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	Camera() {}
	~Camera() {}

	// camera Attributes
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	bool updatemouse;

	// constructor with vectors
	Camera(bool bupdatemouse, vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : updatemouse(bupdatemouse), Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(bool bupdatemouse, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : updatemouse(bupdatemouse), Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = vec3(posX, posY, posZ);
		WorldUp = vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard()
	{
		float velocity = MovementSpeed;
		if (GetAsyncKeyState('W'))
			Position += Front * velocity;
		if (GetAsyncKeyState('S'))
			Position -= Front * velocity;
		if (GetAsyncKeyState('A'))
			Position -= Right * velocity;
		if (GetAsyncKeyState('D'))
			Position += Right * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void UpdateInput(HWND hwnd)
	{
		POINT pt;
		RECT window_rect;
		GetWindowRect(hwnd, &window_rect);
		GetCursorPos(&pt);

		POINT cpt;
		cpt.x = (window_rect.right / 2);
		cpt.y = (window_rect.bottom / 2);
		float xdiff = pt.x - cpt.x;
		float ydiff = pt.y - cpt.y;
		if (updatemouse) {
			SetCursorPos(cpt.x, cpt.y);
			ProcessKeyboard();
			ProcessMouseMovement(xdiff, -ydiff, true);
		}
	}

	void Look(HWND hwnd)
	{
		UpdateInput(hwnd);
		vec3 Dir = Position + Front;
		gluLookAt(Position.x, Position.y, Position.z, Dir.x, Dir.y, Dir.z, 0.f, 1.f, 0.f);
	}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		vec3 front;
		front.x = cosf(Yaw * DTOR) * cosf(Pitch * DTOR);
		front.y = sinf(Pitch * DTOR);
		front.z = sinf(Yaw  * DTOR) * cosf(Pitch * DTOR);
		Front = normalize(front);
		// also re-calculate the Right and Up vector
		Right = normalize(cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = normalize(cross(Right, Front));
	}
};