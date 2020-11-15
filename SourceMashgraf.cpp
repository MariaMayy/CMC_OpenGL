#define GLEW_STATIC // используем статическую версию библиотеки GLEW
#include <GL/glew.h> // подключаем заголовочные файлы библиотек
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

// Вызывается, когда используется клавиатура; на вход - указатель на окно, число - нажатая клавиша, действие над клавишей и модификатор(shift, alt)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main(void)
{
	// Инициализация GLFW
	glfwInit();

	// Настраиваем GLFW, задаем версию 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Устанавливаем профайл, для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Запрещаем изменять размер окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// создаем окно размера 800 на 600, функция возвращает указатель на объект
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

	int width, height; // ширина и высота окна
	// задаем размеры отрисовываемого окна
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height); // устанвливаем координаты относительно окна - левый нижний угол и правый верхний

	// игровой цикл, работает, пока не остановим
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // проверяет, были ли вызваны какие-либо события (ввод с клавиатуры/перемещение мыши)

		glClearColor(0.5f, 0.5f, 0.8f, 0.6f); // устанавливаем цвет
		glClear(GL_COLOR_BUFFER_BIT); // закрашиваем

		glfwSwapBuffers(window); // меняем буферы местами
	}

	glfwTerminate();



	return 0;

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// если нажата ESC, то окно закроется  
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}