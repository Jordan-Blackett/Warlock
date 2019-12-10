#include "btpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace BlackThorn {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}