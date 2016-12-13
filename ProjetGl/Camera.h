#pragma once
class Camera
{
private:
	glm::vec4 position;
	float speed;
	float angle;
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;
	float mouseSpeed = 0.1f;
	glm::mat4 CamViewMatrice;
	glm::mat4 projection;

public:
	Camera();

	void execute(GLFWwindow *window);

	glm::mat4 getCamView() { return CamViewMatrice; };
	glm::mat4 getProjection() { return projection; };

//	glm::mat4 lookAt(glm::vec3 eye, glm::vec3 at, glm::vec3 up);

	~Camera();
};

