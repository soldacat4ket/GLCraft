#include "Shader.h"

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"

// need to learn more about texture slots before i can remove this ugly thing
static unsigned int s_OpenTextureSlot = 0;

Shader::Shader(const std::filesystem::path& p_VertexPath, const std::filesystem::path& p_FragmentPath)
{
    unsigned int VertexShader = CompileShader(p_VertexPath.string().c_str(), GL_VERTEX_SHADER);
    unsigned int FragmentShader = CompileShader(p_FragmentPath.string().c_str(), GL_FRAGMENT_SHADER);

    m_RendererID = CompileProgram(VertexShader, FragmentShader);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::SetSampler2D(const std::string& p_Name, const Texture& p_Tex) const
{
    p_Tex.Bind(s_OpenTextureSlot);
    SetInt(p_Name, s_OpenTextureSlot);
    s_OpenTextureSlot++;
}

void Shader::SetBool(const std::string& p_Name, bool p_Value) const
{
    glUniform1i(glGetUniformLocation(m_RendererID, p_Name.c_str()), (int)p_Value);
}

void Shader::SetInt(const std::string& p_Name, int p_Value) const
{
    glUniform1i(glGetUniformLocation(m_RendererID, p_Name.c_str()), p_Value);
}

void Shader::SetFloat(const std::string& p_Name, float p_Value) const
{
    glUniform1f(glGetUniformLocation(m_RendererID, p_Name.c_str()), p_Value);
}

void Shader::SetVec2(const std::string& p_Name, const glm::vec2 p_Vec) const
{
    glUniform2fv(glGetUniformLocation(m_RendererID, p_Name.c_str()), 1, glm::value_ptr(p_Vec));
}

void Shader::SetVec3(const std::string& p_Name, const glm::vec3 p_Vec) const
{
    glUniform3fv(glGetUniformLocation(m_RendererID, p_Name.c_str()), 1, glm::value_ptr(p_Vec));
}

void Shader::SetVec4(const std::string& p_Name, const glm::vec4 p_Vec) const
{
    glUniform4fv(glGetUniformLocation(m_RendererID, p_Name.c_str()), 1, glm::value_ptr(p_Vec));
}

void Shader::SetMat2(const std::string& p_Name, const glm::mat2& p_Mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_RendererID, p_Name.c_str()), 1, GL_FALSE, glm::value_ptr(p_Mat));
}

void Shader::SetMat3(const std::string& p_Name, const glm::mat3& p_Mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_RendererID, p_Name.c_str()), 1, GL_FALSE, glm::value_ptr(p_Mat));
}

void Shader::SetMat4(const std::string& p_Name, const glm::mat4& p_Mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, p_Name.c_str()), 1, GL_FALSE, glm::value_ptr(p_Mat));
}

void Shader::CheckCompilerErrors(const unsigned int p_Shader, const std::string& p_Type)
{
    int Success;
    char InfoLog[1024];
    if(p_Type != "PROGRAM")
    {
        glGetShaderiv(p_Shader, GL_COMPILE_STATUS, &Success);
        if(!Success)
        {
            glGetShaderInfoLog(p_Shader, 1024, NULL, InfoLog);
            LOG_ERROR("Shader compilation error-> {}:\n{}", p_Type, InfoLog);
            ASSERT(false);
        }
    }
    else
    {
        glGetProgramiv(p_Shader, GL_LINK_STATUS, &Success);
        if(!Success)
        {
            glGetProgramInfoLog(p_Shader, 1024, NULL, InfoLog);
            LOG_ERROR("Program compilation error-> {}:\n{}", p_Type, InfoLog);
            ASSERT(false);
        }
    }
}

std::string Shader::GetShaderTypeAsString(const GLenum p_Type)
{
    switch(p_Type)
    {
        case GL_VERTEX_SHADER:
            return "Vertex";
            break;
        case GL_FRAGMENT_SHADER:
            return "Fragment";
            break;
        case GL_GEOMETRY_SHADER:
            return "Geometry";
            break;
        default:
            LOG_ERROR("Unknown shader type: {}", p_Type);
            return "Unknown Shader Type";
    }
}

unsigned int Shader::CompileShader(const std::filesystem::path p_Path, const GLenum p_Type)
{
    unsigned int TempShader;

    // really ugly but itll have to work for now
    std::ifstream FileHandle(p_Path);
    ASSERT(FileHandle.is_open());
    std::stringstream ShaderCodeBuffer;
    ShaderCodeBuffer << FileHandle.rdbuf();
    std::string ShaderCodeString = ShaderCodeBuffer.str();
    const char* ShaderCode = ShaderCodeString.c_str();

    TempShader = glCreateShader(p_Type);

    glShaderSource(TempShader, 1, &ShaderCode, NULL);
    glCompileShader(TempShader);

    CheckCompilerErrors(TempShader, GetShaderTypeAsString(p_Type));

    return TempShader;
}

unsigned int Shader::CompileProgram(const unsigned int p_Vertex, const unsigned int p_Fragment)
{
    unsigned int TempProgram = glCreateProgram();

    glAttachShader(TempProgram, p_Vertex);
    glAttachShader(TempProgram, p_Fragment);

    glLinkProgram(TempProgram);

    CheckCompilerErrors(TempProgram, "PROGRAM");
    return TempProgram;
}
