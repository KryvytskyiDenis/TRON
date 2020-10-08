#include "trpch.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        TR_CRITICAL(message);
        return;
    case GL_DEBUG_SEVERITY_MEDIUM:
        ERROR(message);
        return;
    case GL_DEBUG_SEVERITY_LOW:
        TR_WARN(message);
        return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        TR_INFO(message);
        return;
    }

    TR_ASSERT(false, "Unknown severity level!");
}

int main()
{
    Log::Init();

    if (!glfwInit())
    {
        TR_ASSERT(false, "Failed to initialize GLFW!");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Tron", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        TR_ASSERT(false, "Failed to create window");
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        TR_ASSERT(false,"Failed to initialize Glad!");
        return -1;
    }

    // Enable OpenGL debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    // Info about graphics vendor
    TR_INFO("OpenGL info:\n Vendor: {0}\n Renderer: {1}\n Version: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));

    // Set GLFW callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    });
    //---------------

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float positions[] = {
        // pos       // tex
         -.5f, .5f,  0.0f, 1.0f, // top left
         0.5f, -.5f, 1.0f, 0.0f, // bottom right
         -.5f, -.5f, 0.0f, 0.0f, // bottom left
         .5f, 0.5f,  1.0f, 1.0f // top right
    };

    uint32_t indices[] = {
        0, 1, 2,
        0, 1, 3
    };

    VertexArray va;

    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout vbLayout = {
        { BufferAttributeType::Float2 },
        { BufferAttributeType::Float2 }
    };
    vb.SetLayout(vbLayout);

    va.AddVertexBuffer(&vb);

    IndexBuffer ib(indices, 6);

    // Shaders
    ShaderManager shaderManager;
    std::shared_ptr<Shader> shader = shaderManager.Load("PosColorTexShader", "res/shaders/shader.vert", "res/shaders/shader.frag");
    shader->Bind();
    shader->SetVector4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);

    // Textures
    Texture texture("res/textures/wizzard.png");
    texture.Bind();
    // set the number of texture slot
    shader->SetInteger("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader->Unbind();

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