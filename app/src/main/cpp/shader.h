#pragma once

#include <GLES2/gl2.h>
#include <vector>

struct ShaderManager {
    static bool CompileShader(GLuint*, const GLenum, const std::vector<uint8_t>&);
    static bool CompileShader(GLuint*, const GLenum, const GLchar*, const size_t);
    static bool CompileShader(GLuint*, const GLenum, const char*);
    static bool LinkProgram(const GLuint);
    static bool ValidateProgram(const GLuint);
};