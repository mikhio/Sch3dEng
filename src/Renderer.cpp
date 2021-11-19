#include "Renderer.hpp"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function)
{
    if (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") --> " << function << " ";
        return false;
    }

    return true;
}

void Renderer::Draw(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const
{
    shader.Bind();
    va.Bind();

    // std::cout << (unsigned int)(vb.GetSize() / va.GetStride()) << std::endl;

    // GLCall(glEnable(GL_DEPTH_TEST));
    // GLCall(glDepthFunc(GL_LEQUAL));     

    GLCall(glDrawArrays(GL_TRIANGLES, 0, (unsigned int)(vb.GetSize() / va.GetStride())));

    // GLCall(glDisable(GL_DEPTH_TEST));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
    // std::cout << ib.GetCount() << std::endl;

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    // GLCall(glClearColor(87.f/255.f, 195.f/255.f, 235.f/255.f, 1.f));
}