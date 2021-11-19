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



struct MouseState {
    double xpos, ypos;
    bool rightBTN;
    bool leftBTN;
};

struct MouseCameraController {
    float clickedX, clickedY;
    float sens;
    bool isShiftClicked;

    glm::vec3 clickedTrans;
    glm::mat4 clickedRot;
};



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

        float WindowWidth = 800.f;
        float WindowHeight = 600.f;

        float AspectRatio = (float)WindowWidth / (float)WindowHeight;

        window = glfwCreateWindow((unsigned int)WindowWidth, (unsigned int)WindowHeight, "3DEngGL", NULL, NULL);

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

        // glfwSetCursorPosCallback(window, cursor_position_callback);
        // glfwSetMouseButtonCallback(window, mouse_button_callback);


        GLCall(std::cout << "INFO: OpenGL" << glGetString(GL_VERSION) << std::endl);

        float positions[] = {
            -1.f, -1.f,  1.f, -1.f, -1.f,  1.f,
             1.f, -1.f,  1.f,  1.f, -1.f,  1.f,
             1.f,  1.f,  1.f,  1.f,  1.f,  1.f,
            -1.f,  1.f,  1.f, -1.f,  1.f,  1.f,

            -1.f, -1.f, -1.f, -1.f, -1.f, -1.f,
             1.f, -1.f, -1.f,  1.f, -1.f, -1.f,
             1.f,  1.f, -1.f,  1.f,  1.f, -1.f,
            -1.f,  1.f, -1.f, -1.f,  1.f, -1.f,
        };

        float CubeMesh[] = {
            // FRONT
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

            // BACK
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

            // RIGHT
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 

             // LEFT
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

            // BOTTOM
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

            // TOP
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
        };

        unsigned int wireframeIndices[] = {
            // FRONT
            0, 1, 1, 2, 2, 0,
            2, 3, 3, 0, 0, 2,

            // RIGHT
            1, 5, 5, 6, 6, 1,
            6, 2, 2, 1, 1, 6,

            // BACK
            5, 4, 4, 7, 7, 5,
            7, 6, 6, 5, 5, 7,

            // LEFT
            4, 0, 0, 3, 3, 4,
            3, 7, 7, 4, 4, 3,

            // BOTTOM
            4, 5, 5, 1, 1, 4,
            1, 0, 0, 4, 4, 1,

            // TOP
            3, 2, 2, 6, 6, 3,
            6, 7, 7, 3, 3, 6
        };

        unsigned int indices[] = {
            // FRONT
            0, 1, 2,
            2, 3, 0,

            // RIGHT
            1, 5, 6,
            6, 2, 1,

            // BACK
            5, 4, 7,
            7, 6, 5,

            // LEFT
            4, 0, 3,
            3, 7, 4,

            // BOTTOM
            4, 5, 1,
            1, 0, 4,

            // TOP
            3, 2, 6,
            6, 7, 3
        };

        // GLCall(glEnable(GL_BLEND));
        // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        MouseCameraController CamController;
        MouseState MainMouse;


        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));


        VertexArray va;
        VertexBuffer vb(CubeMesh, sizeof(CubeMesh));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, sizeof(indices)/sizeof(*indices));

        glm::mat4 proj = glm::perspective(45.f, AspectRatio, 1.0f, 150.f);

        Shader shader("./shaders/Basic.shader");

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);


        float angles[] = {0.f, 0.f, 0.f};

        glm::mat4 rotation(1.f);
        glm::vec3 translation(0.f, 0.f, 0.f);

        CamController.clickedTrans = translation;
        CamController.clickedRot = rotation;
        CamController.sens = 3.f;


        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -4.f));
            view *= glm::translate(glm::mat4(1.f), translation);
            view *= rotation;

            glm::mat4 model = glm::mat4(1.f);

            model = glm::rotate(model, glm::radians(angles[0]), glm::vec3(glm::vec4(1.f, 0.f, 0.f, 0.f)*(model)));
            model = glm::rotate(model, glm::radians(angles[1]), glm::vec3(glm::vec4(0.f, 1.f, 0.f, 0.f)*(model)));
            model = glm::rotate(model, glm::radians(angles[2]), glm::vec3(glm::vec4(0.f, 0.f, 1.f, 0.f)*(model)));
            

            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.SetUniformMat4f("u_Model", model);


            // renderer.Draw(va, ib, shader);
            renderer.Draw(va, vb, shader);

            glfwGetCursorPos(window, &MainMouse.xpos, &MainMouse.ypos);

            int leftBtnState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

            if (leftBtnState == GLFW_PRESS && MainMouse.leftBTN == GLFW_RELEASE)
            {
                CamController.clickedX = MainMouse.xpos;
                CamController.clickedY = MainMouse.ypos;
                CamController.clickedTrans = translation;
                CamController.clickedRot = rotation;

                CamController.isShiftClicked = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
            }

            MainMouse.leftBTN = leftBtnState;

            if (MainMouse.leftBTN == GLFW_PRESS)
            {
                if (CamController.isShiftClicked)
                {
                    glm::vec2 movVec((float)MainMouse.xpos - CamController.clickedX, CamController.clickedY - (float)MainMouse.ypos);
                    glm::vec2 axis = glm::vec2(glm::vec4(movVec, 0.f, 0.f) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0.f, 0.f, -1.f)));

                    float angle = CamController.sens * glm::length(glm::vec2(movVec.x/WindowWidth, movVec.y/WindowHeight));

                    if (angle != 0)
                        rotation = glm::rotate(CamController.clickedRot, angle, glm::vec3(glm::vec4(axis, 0.f, 0.f) * CamController.clickedRot));
                } else
                {
                    translation = CamController.clickedTrans;
                    translation.x += CamController.sens * ((float)MainMouse.xpos - CamController.clickedX) / WindowWidth;
                    translation.y += CamController.sens * (CamController.clickedY - (float)MainMouse.ypos) / WindowHeight;
                }
            }



            {
                ImGui::SliderFloat3("Translation", &translation.x, -10.0f, 10.f);
                ImGui::SliderFloat3("Rotation", angles, 0.0f, 360.f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }



            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
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
