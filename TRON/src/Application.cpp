#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

#include "Grid.h"


struct ColorRGBA {
	float r;
	float g;
	float b;
	float alpha;
};

void RenderItem(VertexArray& va, Shader& shader, const VertexBuffer& vb, const VertexBufferLayout& vbl, const ColorRGBA& color);

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	/* Create a windowed mode window and its OpenGL context */
	unsigned int winWidth = 1920;
	unsigned int winHeight = 1080;

	window = glfwCreateWindow(winWidth, winHeight, "TRON", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // vsync

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// Grid
		Grid grid(winWidth, winHeight);
		grid.SetUpPositions();

		// VAO
		VertexArray va;
		
		// VBO
		float* positions = grid.GetPositions();

		VertexBuffer vb(positions, grid.GetPositionsSize() * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);

		//va.AddBuffer(vb, layout);

		// index buffer IBO
		//IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, (float)winWidth, 0.0f, (float)winHeight, -1.0f, 1.0f);

		// Bind shader
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		ColorRGBA gridLines = { 0.6f, 0.6f, 0.6f, 1.0f };
		shader.SetUniformMat4f("u_MVP", proj);

		// Unbound everything
		va.Unbind();
		vb.Unbind();
		//ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		/*glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);*/


		// Strip line vertices (first LightCycle)
		int lightCycleWidth = 7;
		float lightCycle[] = {
			100.0f, 100.0f,
			100.0f, 150.0f,
			300.0f, 150.0f,
			300.0f, 500.0f,
			320.0f, 500.0f,
			320.0f, 650.0f
		};
		VertexBuffer vb2(lightCycle, 12 * sizeof(float));
		VertexBufferLayout layout2;
		layout2.Push<float>(2);
		shader.Bind();
		ColorRGBA red = { 1.0f, 0.0f, 0.0f, 1.0f };
		//
		// Strip line vertices (second  LightCycle)
		float lightCycle2[] = {
			1000.0f, 1000.0f,
			1000.0f, 960.0f,
			870.0f, 960.0f,
			870.0f, 1000.0f,
			700.0f, 1000.0f,
			700.0f, 700.0f
		};
		VertexBuffer vb3(lightCycle2, 12 * sizeof(float));
		VertexBufferLayout layout3;
		layout3.Push<float>(2);
		shader.Bind();
		ColorRGBA green = { 0.0f, 1.0f, 0.0f, 1.0f };
		//

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			// Render here 
			renderer.Clear();

			// Bind VAO and IB
			glLineWidth(1);
			RenderItem(va, shader, vb, layout, gridLines);
			renderer.DrawLines(va,/*ib,*/ shader, grid.GetPositionsSize() / 2);

			// Light Cycle Red
			glLineWidth(lightCycleWidth);
			RenderItem(va, shader, vb2, layout2, red);
			renderer.DrawLineStrip(va, shader, 6);

			// Light Cycle Green
			RenderItem(va, shader, vb3, layout3, green);
			renderer.DrawLineStrip(va, shader, 6);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

void RenderItem(VertexArray& va, Shader& shader,const VertexBuffer& vb,const VertexBufferLayout& vbl,const ColorRGBA& color) {
	va.AddBuffer(vb, vbl);
	shader.Bind();
	shader.SetUniform4f("u_Color", color.r, color.g, color.b, color.alpha);
}