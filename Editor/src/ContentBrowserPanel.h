#pragma once
#include <filesystem>
#include "Aurora/Renderer/Texture.h"
namespace Aurora
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(std::filesystem::path path);
		~ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_currentPath;
		std::filesystem::path m_path;

		Ref<Texture2D> m_directoryIcon;
		Ref<Texture2D> m_fileIcon;
	};
}