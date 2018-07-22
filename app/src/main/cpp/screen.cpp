#include "screen.h"
#include "glcontext.h"

int32_t Screen::width() {
	return GLContext::GetInstance()->GetScreenWidth();
}

int32_t Screen::height() {
	return GLContext::GetInstance()->GetScreenHeight();
}

/*
void Graphics::Load() {
	glcontext = GLContext::GetInstance();

}

void Graphics::Unload() {
	glDeleteProgram(TextureShader);
	TextureShader = 0;
}*/
