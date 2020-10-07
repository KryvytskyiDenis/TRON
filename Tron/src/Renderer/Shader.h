#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const ShaderProgramSource& shaderSource);

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetInteger(const std::string& name, uint32_t value);
    void SetVector2f(const std::string& name, float x, float y);
    void SetVector2f(const std::string& name, glm::vec2& value);
    void SetVector3f(const std::string& name, float x, float y, float z);
    void SetVector3f(const std::string& name, glm::vec3& value);
    void SetVector4f(const std::string& name, float x, float y, float z, float w);
    void SetVector4f(const std::string& name, glm::vec4& value);
    void SetMatrix4(const std::string& name, glm::mat4& value);

private:
    void CompileProgram(const ShaderProgramSource& shaderSource);
    GLuint CompileShader(GLenum type, const std::string& source);

private:
    uint32_t m_RendererId;
};

class ShaderManager
{
public:
    std::shared_ptr<Shader> Load(std::string name, const std::string& vShaderFile, const std::string& fShaderFile);

private:
    std::string ReadFromFile(const std::string& shaderFile);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};