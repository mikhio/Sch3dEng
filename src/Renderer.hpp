#pragma once

#include <GL/glew.h>
#include <iostream>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"

#define ASSERT(x) if (!(x)) { std::cout << "ASSERTION in " << __FILE__ << ":" << __LINE__ << std::endl; throw std::exception(); }
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x))

void GLClearError();
bool GLLogCall(const char* function);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const;
};