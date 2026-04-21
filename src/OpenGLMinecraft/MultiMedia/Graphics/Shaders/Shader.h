#pragma once

#include <string>
#include <filesystem>

#include <glm/glm.hpp>

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/Texture.h"

class Shader
{
public:
    Shader(const std::filesystem::path& p_VertexPath, const std::filesystem::path& p_FragmentPath);

    void Bind() const;

    void SetSampler2D(const std::string& p_Name, const Texture& p_Tex) const;

    void SetBool(const std::string& p_Name, bool p_Value) const;
    void SetInt(const std::string& p_Name, int p_Value) const;
    void SetFloat(const std::string& p_Name, float p_Value) const;

    void SetVec2(const std::string& p_Name, glm::vec2 p_Vec) const;
    void SetVec3(const std::string& p_Name, glm::vec3 p_Vec) const;
    void SetVec4(const std::string& p_Name, glm::vec4 p_Vec) const;

    void SetMat2(const std::string& p_Name, const glm::mat2& p_Mat) const;
    void SetMat3(const std::string& p_Name, const glm::mat3& p_Mat) const;
    void SetMat4(const std::string& p_Name, const glm::mat4& p_Mat) const;

private:
    unsigned int m_RendererID;

    void CheckCompilerErrors(unsigned int p_Shader, const std::string& p_Type);
    std::string GetShaderTypeAsString(GLenum p_Type);

    unsigned int CompileShader(const std::filesystem::path p_Path, GLenum p_Type);
    unsigned int CompileProgram(unsigned int p_Vertex, unsigned int p_Fragment);
};

