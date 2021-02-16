#include "PanelDetailedFrame.h"
#include "PanelFrames.h"
#include "ModuleUI.h"
#include <queue>
#include "Function.h"
#include "Frame.h"

PanelDetailedFrame::PanelDetailedFrame(const std::string& panel_name) : Panel(panel_name)
{
}

PanelDetailedFrame::~PanelDetailedFrame()
{
}
// TODO: update scale with the mouse z
void PanelDetailedFrame::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	Frame* frame = App->ui->panel_frames->frame;
	if (frame != nullptr && !frame->functions.empty()) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
		ShowFunctions(&frame->functions, ImGui::GetCursorPosX(), (ImGui::GetContentRegionAvail().x - 5) * scale, frame->ms);
		ImGui::PopStyleVar();
	}

	ImGui::End();
}

void PanelDetailedFrame::ShowFunctions(std::list<Function*>* functions, float cursorX, float totalSize, unsigned __int32 sizeMs)
{
	ImGui::SetCursorPosX(cursorX);
	for (auto item = functions->begin(); item != functions->end(); ++item) {
		float itemSize = (totalSize * (float)(*item)->ms) / (float)sizeMs;
		ImVec2 beforeCursor = ImGui::GetCursorPos();

		ImGui::PushID(*item);
		ImGui::Button((*item)->name.data(), ImVec2(itemSize, 0));
		ImGui::PopID();

		ImVec2 afterCursor = ImGui::GetCursorPos();

		if (!(*item)->functions.empty()) {
			ShowFunctions(&(*item)->functions, beforeCursor.x, itemSize, (*item)->ms);
		}
	
		if (*item != functions->back()) {
			ImGui::SetCursorPos(ImVec2(beforeCursor.x + itemSize + 1, beforeCursor.y));
		}
	}
}
