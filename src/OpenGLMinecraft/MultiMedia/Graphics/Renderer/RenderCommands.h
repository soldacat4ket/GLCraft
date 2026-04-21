#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"

namespace RenderCommands
{
    void DrawElements(
        GLsizei p_IndexCount, 
        GLenum p_Mode = GL_TRIANGLES, 
        GLenum p_Type = GL_UNSIGNED_INT,
        const GLvoid* p_Indices = nullptr
    ) noexcept;
    void DrawArrays(
        GLsizei p_Count,
        GLenum p_Mode = GL_TRIANGLES,
        GLint p_First = 0
    ) noexcept;
}