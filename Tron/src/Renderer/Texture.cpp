#include "trpch.h"
#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture(const std::string filePath)
    : m_FilePath(filePath)
    , m_RendererId(0)
    , m_Width(0)
    , m_Height(0)
    , m_InternalFormat(GL_RGB)
    , m_ImageFormat(GL_RGB)
    , m_WrapS(GL_CLAMP_TO_EDGE)
    , m_WrapT(GL_CLAMP_TO_EDGE)
    , m_FilterMin(GL_LINEAR)
    , m_FilterMax(GL_LINEAR)
{
    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    TR_ASSERT(data, "Failed to load image!");
    m_Width = width;
    m_Height = height;


    GLenum internalFormat, imageFormat;
    if (channels == 4)
    {
        internalFormat = GL_RGBA;
        imageFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        imageFormat = GL_RGB;
    }

    TR_ASSERT(internalFormat & imageFormat, "Format not supported!");

    m_InternalFormat = internalFormat;
    m_ImageFormat = imageFormat;

    glGenTextures(1, &m_RendererId);
    glBindTexture(GL_TEXTURE_2D, m_RendererId);
    //glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_ImageFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererId);
}

void Texture::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererId);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
