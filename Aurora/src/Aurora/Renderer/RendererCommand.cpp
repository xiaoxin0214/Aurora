#include "pch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace Aurora
{
	RendererAPI* RendererCommand::s_pRendererAPI = new OpenGLRendererAPI();
}