#include "shader.h"
#include "resources.h"

#include "log.h"

bool ShaderManager::CompileShader(GLuint *shader, const GLenum type, const GLchar *source, const size_t size) {
	if (shader == nullptr || source == nullptr || size <= 0) return false;

	*shader = glCreateShader(type);

	glShaderSource(*shader, 1, &source, reinterpret_cast<const GLint *>(&size));
	glCompileShader(*shader);

#if defined(DEBUG)
	GLint length;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        GLchar buffer[length];
        glGetShaderInfoLog(*shader, length, &length, buffer);
        LOGI("Shader compile:\n%s", buffer);
    }
#endif

	GLint status;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		glDeleteShader(*shader);
		return false;
	}

	return true;
}

bool ShaderManager::CompileShader(GLuint *shader, const GLenum type, const std::vector<uint8_t> &data) {
	return !data.empty() && ShaderManager::CompileShader(shader, type, (const GLchar *)&data[0], data.size());
}

bool ShaderManager::CompileShader(GLuint *shader, const GLenum type, const char *fileName) {
	return ShaderManager::CompileShader(shader, type, ResourceManager::ReadFile(fileName));
}

bool ShaderManager::LinkProgram(const GLuint program) {
	GLint status;

	glLinkProgram(program);

#if defined(DEBUG)
	GLint length;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        GLchar buffer[length];
        glGetShaderInfoLog(program, length, &length, buffer);
        LOGI("Program link:\n%s", buffer);
    }
#endif

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == 0) {
		LOGI("Program link failed\n");
		return false;
	}

	return true;
}

bool ShaderManager::ValidateProgram(const GLuint program) {
	glValidateProgram(program);

#if defined(DEBUG)
	GLint length;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        GLchar buffer[length];
        glGetShaderInfoLog(program, length, &length, buffer);
        LOGI("Program validate:\n%s", buffer);
    }
#endif

    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    return status != 0;
}
