#include "trpch.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static std::string ReadFile(const std::string& filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in || std::ios::binary);
    if (in)
    {
        in.seekg(0, in.end);
        result.resize(in.tellg());
        in.seekg(0, in.beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        std::cout << "Could not open file " << filepath << std::endl;
        TR_ASSERT(false);
    }

    return result;
}

static ShaderProgramSource ParseShader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    ShaderProgramSource shaderProgram;

    shaderProgram.VertexSource = ReadFile(vertexFilePath);
    shaderProgram.FragmentSource = ReadFile(fragmentFilePath);

    return shaderProgram;
}

static GLuint CompileShader(GLenum type, const std::string& source)
{
    GLuint shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    GLint isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(shaderId);

        std::cout << "Failed shader compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
        std::cout << std::string(infoLog.data()) << std::endl;

        return -1;
    }

    return shaderId;
}

static GLuint CreateShader(const ShaderProgramSource shaderProgramSource)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, shaderProgramSource.VertexSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, shaderProgramSource.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        std::cout << "Failed program lingking:" << std::endl;
        std::cout << std::string(infoLog.data()) << std::endl;

        return -1;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

    // Enable opengl debug
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
        {BufferAttributeType::Float2}
    };
    vb.SetLayout(vbLayout);
    va.AddVertexBuffer(&vb);

    IndexBuffer ib(indices, 6);

    // Shaders
    ShaderProgramSource source = ParseShader("res/shaders/shader.vert", "res/shaders/shader.frag");
    GLuint shaderProgram = CreateShader(source);
    glUseProgram(shaderProgram);
    // -----
    int location = glGetUniformLocation(shaderProgram, "u_Color");
    TR_ASSERT(location != -1);
    glUniform4f(location, 0.0f, 1.0f, 0.0f, 1.0f);

    //------

    va.Unbind();
    glUseProgram(0);
    /*glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        glUseProgram(shaderProgram);
        va.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}