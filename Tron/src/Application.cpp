#include "trpch.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        CRITICAL(message);
        TR_ASSERT(false);
        return;
    case GL_DEBUG_SEVERITY_MEDIUM:
        ERROR(message);
        return;
    case GL_DEBUG_SEVERITY_LOW:
        WARN(message);
        return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        INFO(message);
        return;
    }

    TR_ASSERT(false, "Unknown severity level!");
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        TR_ASSERT(false);
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Tron", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        TR_ASSERT(false);
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize Glad!" << std::endl;
        TR_ASSERT(false);
        return -1;
    }

    // Enable OpenGL debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    // Info about graphics vendor
    std::cout << "OpenGL info:" << std::endl;
    std::cout << " Vendor:" << glGetString(GL_VENDOR) << std::endl;
    std::cout << " Renderer:" << glGetString(GL_RENDERER) << std::endl;
    std::cout << " Version:" << glGetString(GL_VERSION) << std::endl;

    // Set GLFW callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    });
    //---------------

    float positions[] = {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f
    };

    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    VertexArray va;

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout vbLayout = {
        { BufferAttributeType::Float2 }
    };
    vb.SetLayout(vbLayout);

    va.AddVertexBuffer(&vb);

    IndexBuffer ib(indices, 6);

    // Shaders
    ShaderManager shaderManager;
    std::shared_ptr<Shader> shader = shaderManager.Load("PosColorShader", "res/shaders/shader.vert", "res/shaders/shader.frag");
    shader->Bind();
    shader->SetVector4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);

    Renderer renderer;

    while (!glfwWindowShouldClose(window))
    {
        renderer.PreRender();

        glfwPollEvents();

        renderer.Draw(va, ib, shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}