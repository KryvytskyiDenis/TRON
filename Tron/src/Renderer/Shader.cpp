#include "trpch.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const ShaderProgramSource& shaderSource)
{
    CompileProgram(shaderSource);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererId);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetInteger(const std::string& name, uint32_t value)
{
    glUniform1i(glGetUniformLocation(m_RendererId, name.c_str()), value);
}

void Shader::SetVector2f(const std::string& name, float x, float y)
{
    glUniform2f(glGetUniformLocation(m_RendererId, name.c_str()), x, y);
}

void Shader::SetVector2f(const std::string& name, glm::vec2& value)
{
    glUniform2f(glGetUniformLocation(m_RendererId, name.c_str()), value.x, value.y);
}

void Shader::SetVector3f(const std::string& name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(m_RendererId, name.c_str()), x, y, z);
}

void Shader::SetVector3f(const std::string& name, glm::vec3& value)
{
    glUniform3f(glGetUniformLocation(m_RendererId, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVector4f(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_RendererId, name.c_str()), x, y, z, w);
}

void Shader::SetVector4f(const std::string& name, glm::vec4& value)
{
    glUniform4f(glGetUniformLocation(m_RendererId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const std::string& name, glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_RendererId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::CompileProgram(const ShaderProgramSource& shaderSource)
{
    m_RendererId = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, shaderSource.VertexSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.FragmentSource);

    glAttachShader(m_RendererId, vs);
    glAttachShader(m_RendererId, fs);
    glLinkProgram(m_RendererId);

    GLint isLinked = 0;
    glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_RendererId);

        TR_ERROR("Failed program lingking: {0}", infoLog.data());
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

GLuint Shader::CompileShader(GLenum type, const std::string& source)
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

        TR_ERROR("Failed shader compile: {0} {1} {2}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", " shader:", infoLog.data());

        return -1;
    }

    return shaderId;
}

//----------------------------------------------------------------

std::shared_ptr<Shader> ShaderManager::Load(std::string name, const std::string& vShaderFile, const std::string& fShaderFile)
{
    ShaderProgramSource shaderProgram;

    shaderProgram.VertexSource = ReadFromFile(vShaderFile);
    shaderProgram.FragmentSource = ReadFromFile(fShaderFile);

    auto shader = std::make_shared<Shader>(shaderProgram);
    m_Shaders[name] = shader;

    return shader;
}

std::string ShaderManager::ReadFromFile(const std::string& shaderFile)
{
    std::string result;
    std::ifstream in(shaderFile, std::ios::in || std::ios::binary);
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
        TR_ASSERT(false, "Could not open file ");
    }

    return result;
}