#pragma once
class Camera
{
private:
	glm::vec3 position = glm::vec3(0, 0, 5);
	float speed;
	glm::mat4 CamViewMatrice;
	glm::mat4 projection;

public:
	Camera();

	void execute(GLFWwindow *window);
	glm::mat4 getCamView() { return CamViewMatrice; };
	glm::mat4 getProjection() { return projection; };

	~Camera();
};

