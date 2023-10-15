#include "ContentBrowserPanel.h"
#include "imgui.h"
namespace Aurora
{
	ContentBrowserPanel::ContentBrowserPanel(std::filesystem::path path):m_path(path),m_currentPath(path)
	{
		m_directoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_fileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{

	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin(u8"资源管理器");

		if (m_currentPath != m_path)
		{
			if (ImGui::Button(u8".."))
			{
				m_currentPath = m_currentPath.parent_path();
			}
		}

		static float padding = 8.0f;
		static float thumbnailSize = 64.0f;// 缩略图大小
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for(auto&dirEntry:std::filesystem::directory_iterator(m_currentPath))
		{
			const auto& path = dirEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = dirEntry.is_directory() ? m_directoryIcon : m_fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dirEntry.is_directory())
					m_currentPath /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

}