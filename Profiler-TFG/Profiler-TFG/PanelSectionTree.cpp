#include "PanelSectionTree.h"

PanelSectionTree::PanelSectionTree(const std::string& panel_name) : Panel(panel_name)
{
}

PanelSectionTree::~PanelSectionTree()
{
}

void PanelSectionTree::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::End();
}
