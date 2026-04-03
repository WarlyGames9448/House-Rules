#include "fuzepch.h"

#include "Renderer/RendererCommand.h"
#include "Plataform/OpenGL/OpenGLRendererAPI.h"

namespace Fuze {
RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}
