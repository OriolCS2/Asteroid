#include "PanelFrames.h"

PanelFrames::PanelFrames(const std::string& panel_name) : Panel(panel_name)
{

}

PanelFrames::~PanelFrames()
{
}

void PanelFrames::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::End();
}
