#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

int main(void)
{
    try {
        GLFWwindow *window;
        if (!glfwInit())
        {
            std::cout << "ERROR: GLFW didn't init" << std::endl;
            throw std::exception();
        }

        const char* glsl_version = "#version 330";

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        std::cout << "MACHINE: APPLE" << std::endl;
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        unsigned int WindowWidth = 800;
        unsigned int WindowHeight = 600;

        // float AspectRatio = (float)WindowWidth / (float)WindowHeight;

        window = glfwCreateWindow(WindowWidth, WindowHeight, "3DEngGL", NULL, NULL);

        if (!window)
        {
            std::cout << "ERROR: Window didn't create" << std::endl;
            glfwTerminate();
            throw std::exception();
        }

        unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
        unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
        std::cout << "GLSL: " << major << "." << minor << std::endl;
        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK) {
            std::cout << "ERROR: GLEW didn't init" << std::endl;
        }

        GLCall(std::cout << "INFO: OpenGL" << glGetString(GL_VERSION) << std::endl);

        float positions[] = {
            -50.f, -50.f, 0.0f, 0.0f,
             50.f, -50.f, 1.0f, 0.0f,
             50.f,  50.f, 1.0f, 1.0f,
            -50.f,  50.f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));


        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);
        unsigned int ibo;
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

        glm::mat4 proj = glm::ortho(0.f, (float)WindowWidth, 0.f, (float)WindowHeight, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));

        Shader shader("./shaders/Basic.shader");

        Texture texture("./textures/R2-D2_Droid.png");
        texture.Bind();

        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // bool show_demo_window = true;
        // bool show_another_window = false;
        // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        glm::vec3 translationA(200.f, 200.f, 0);
        glm::vec3 translationB(400.f, 200.f, 0);

        float t = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                // static float f = 0.0f;

                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, (float)WindowWidth);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, (float)WindowWidth);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }



            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
            t += 0.01f;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
    }
    catch (const std::exception&)
    {
        std::cout << "EXCEPTION" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
