#pragma once
#include "Camera.h"
class FlyCamera : public Camera
{
public:
	FlyCamera();
	~FlyCamera();

	void Update(float deltaTime);
	void SetSpeed(float speed);

protected:
	float speed;
	glm::vec3 up;
};

