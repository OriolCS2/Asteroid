#include "PanelDetailedFrame.h"
#include "PanelFrames.h"
#include "ModuleUI.h"
#include <queue>
#include "Function.h"
#include "Frame.h"
#include "ModuleInput.h"

PanelDetailedFrame::PanelDetailedFrame(const std::string& panel_name) : Panel(panel_name)
{
}

PanelDetailedFrame::~PanelDetailedFrame()
{
}

void PanelDetailedFrame::PanelLogic()
{
	float mouseZ = App->input->GetMouseZ();
	if (mouseZ != 0.0F) {
		scale = Clamp(scale + mouseZ * 0.5F, 1.0F, 10.0F);
	}

	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	Frame* frame = App->ui->panel_frames->frame;
	if (frame != nullptr && !frame->functions.empty()) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
		ShowFunctions(&frame->functions, ImGui::GetCursorPosX(), (ImGui::GetContentRegionAvail().x - 5) * scale, frame->ms);
		ImGui::PopStyleVar();
	}

	ImGui::End();
}

void PanelDetailedFrame::ShowFunctions(std::list<Function*>* functions, float cursorX, float totalSize, double sizeMs)
{
	ImGui::SetCursorPosX(cursorX);
	for (auto item = functions->begin(); item != functions->end(); ++item) {
		float itemSize = (totalSize * (float)(*item)->ms) / (float)sizeMs;
		ImVec2 beforeCursor = ImGui::GetCursorPos();

		if (function_selected == *item) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
		}
		ImGui::PushID(*item);
		if (ImGui::Button((*item)->name.data(), ImVec2(itemSize, 0))) {
			if (function_selected == *item) {
				ImGui::PopStyleColor();
			}
			App->ui->OnFunctionSelected(*item);
		}
		else if (function_selected == *item) {
			ImGui::PopStyleColor();
		}
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
