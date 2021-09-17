#include "Window.h"
#include "Errors.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

int Window::Create(const std::string& windowName, int screenWidth, int screenHeight, unsigned int windowFlags) {
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (windowFlags & Invisible) flags |= SDL_WINDOW_HIDDEN;
    if (windowFlags & Fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (windowFlags & Borderless) flags |= SDL_WINDOW_BORDERLESS;

    sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				 screenWidth, screenHeight, flags);
    if (sdlWindow == nullptr) {
	FatalError("SDL_Window could not be created.");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
    if (glContext == nullptr) {
	FatalError("SDL_GLContext could not be created.");
    }

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK) {
	FatalError("Could not initialize glew.");
    }

    std::printf("*** OpenGL version: %s ***\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    SDL_GL_SetSwapInterval(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

    
void Window::SwapBuffer() {
    SDL_GL_SwapWindow(sdlWindow);
}
