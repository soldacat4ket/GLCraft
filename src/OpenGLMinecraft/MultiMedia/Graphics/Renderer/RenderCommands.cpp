#include "RenderCommands.h"

void RenderCommands::DrawElements(const GLsizei p_IndexCount,
                                  const GLenum p_Mode,
                                  const GLenum p_Type,
                                  const GLvoid* p_Indices) noexcept
{
    glDrawElements(p_Mode, p_IndexCount, p_Type, p_Indices);
}

void RenderCommands::DrawArrays(const GLsizei p_Count, 
                                const GLenum p_Mode, 
                                const GLint p_First) noexcept
{
    glDrawArrays(p_Mode, p_First, p_Count);
}
