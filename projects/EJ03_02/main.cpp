#include <glad/glad.h>

#include "engine/input.hpp"
#include "engine/window.hpp"
#include "engine/shader.hpp"

#include <iostream>
#include <GLFW\glfw3.h>

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {
	float vertices[] = {
		0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = {
		0, 2, 1
	};

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}


void render(uint32_t VAO, const Shader& shader) {
	glClear(GL_COLOR_BUFFER_BIT);

	// Color en base al tiempo que ha pasado. El color ira incrementandose segun el valor rgb y luego decrementando para dar un efecto continuo.

	double color = glfwGetTime();
	bool inverse = false;
	while (color > 15) {
		color -= 15;
		inverse = !inverse;
	}

	double rgb[3];
	for (int i = 0; i < 3; i++) {
		if (color > 5) {
			if (inverse) {
				rgb[i] = 0;
			}
			else {
				rgb[i] = 5;
			}
			color -= 5;
		}
		else {
			if (inverse) {
				rgb[i] = 5 - color;
			}
			else {
				rgb[i] = color;

			}
			color = 0;
		}
		rgb[i] = rgb[i] / 5;
	}

	shader.use();
	shader.set("addColor", rgb[0], rgb[1], rgb[2]); // Asignamos el valor a la variable uniforme que se anadio en la practica AG03 en el shader de fragmento

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO, EBO;
	const uint32_t VAO = createVertexData(&VBO, &EBO);
	const Shader shader("../projects/EJ03_02/vertex.vs", "../projects/EJ03_02/fragment.fs");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (window->alive()) {
		render(VAO, shader);
		window->frame();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}