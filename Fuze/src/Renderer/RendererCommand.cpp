#include "fuzepch.h"

#include "Renderer/RendererCommand.h"
#include "Plataform/OpenGL/OpenGLRendererAPI.h"

namespace Fuze {
Scope<RendererAPI> RendererCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}
