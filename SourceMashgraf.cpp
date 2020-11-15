#define GLEW_STATIC // ���������� ����������� ������ ���������� GLEW
#include <GL/glew.h> // ���������� ������������ ����� ���������
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

// ����������, ����� ������������ ����������; �� ���� - ��������� �� ����, ����� - ������� �������, �������� ��� �������� � �����������(shift, alt)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main(void)
{
	// ������������� GLFW
	glfwInit();

	// ����������� GLFW, ������ ������ 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// ������������� �������, ��� �������� ��������� ��������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������� �������� ������ ����
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// ������� ���� ������� 800 �� 600, ������� ���������� ��������� �� ������
	GLFWwindow* window = glfwCreateWindow(800, 600, "MashGraf", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Oops, couldn't create GLFW window:c" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Oops, couldn't initialized GLEW" << endl;
		return -1;
	}

	int width, height; // ������ � ������ ����
	// ������ ������� ��������������� ����
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height); // ������������ ���������� ������������ ���� - ����� ������ ���� � ������ �������

	// ������� ����, ��������, ���� �� ���������
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // ���������, ���� �� ������� �����-���� ������� (���� � ����������/����������� ����)

		glClearColor(0.5f, 0.5f, 0.8f, 0.6f); // ������������� ����
		glClear(GL_COLOR_BUFFER_BIT); // �����������

		glfwSwapBuffers(window); // ������ ������ �������
	}

	glfwTerminate();



	return 0;

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// ���� ������ ESC, �� ���� ���������  
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}