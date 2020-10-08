#pragma once

class Texture
{
public:
    Texture(const std::string filePath);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    void SetInternalFormat(uint32_t internalFormat) { m_InternalFormat = internalFormat; }
    void SetImageFormat(uint32_t imageFormat) { m_ImageFormat = imageFormat; }

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }

private:
    std::string m_FilePath;
    uint32_t m_RendererId;
    uint32_t m_Width;
    uint32_t m_Height;

    uint32_t m_InternalFormat;
    uint32_t m_ImageFormat;

    uint32_t m_WrapS;
    uint32_t m_WrapT;
    uint32_t m_FilterMin;
    uint32_t m_FilterMax;
};

