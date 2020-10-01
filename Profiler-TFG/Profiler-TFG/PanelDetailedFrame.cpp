#include "PanelDetailedFrame.h"

PanelDetailedFrame::PanelDetailedFrame(const std::string& panel_name) : Panel(panel_name)
{
}

PanelDetailedFrame::~PanelDetailedFrame()
{
}

void PanelDetailedFrame::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::End();
}
