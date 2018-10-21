#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//  Basically, there is all stuff for the debug and error handling 
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear();
	void Draw(const VertexArray& va,/* const IndexBuffer& ib, */const Shader& shader, const int& size) const;
	void DrawLines(const VertexArray& va, /*const IndexBuffer& ib, */const Shader& shader, const int& size) const;
	void DrawLineStrip(const VertexArray& va, /*const IndexBuffer& ib, */const Shader& shader, const int& size) const;
};