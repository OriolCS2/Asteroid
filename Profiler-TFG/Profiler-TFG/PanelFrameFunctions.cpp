#include "PanelSectionFunctions.h"

PanelSectionFunctions::PanelSectionFunctions(const std::string& panel_name) : Panel(panel_name)
{
}

PanelSectionFunctions::~PanelSectionFunctions()
{
}

void PanelSectionFunctions::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::End();
}
