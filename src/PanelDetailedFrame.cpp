#include "PanelDetailedFrame.h"
#include "PanelFrames.h"
#include "ModuleProfile.h"
#include "ModuleUI.h"
#include "Color.h"
#include <queue>
#include "Function.h"
#include "Frame.h"
#include "ModuleInput.h"
#include "imgui/imgui_internal.h"

PanelDetailedFrame::PanelDetailedFrame(const std::string& panel_name) : Panel(panel_name)
{
}

PanelDetailedFrame::~PanelDetailedFrame()
{
}

void PanelDetailedFrame::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	float mouseZ = App->input->GetMouseZ();
	if (mouseZ != 0.0F && ImGui::IsWindowHovered()) {
		scale = max(1.0F, (scale + mouseZ * sqrt(scale)));
	}

	Frame* frame = App->ui->panel_frames->frame;
	if (frame != nullptr && !frame->functions.empty()) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
		ShowFunctions(&frame->functions, ImGui::GetCursorPosX(), (ImGui::GetContentRegionAvail().x - 5) * scale, frame->ms, ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - 5) * scale);
		ImGui::PopStyleVar();
	}

	ImGui::End();
}

#define ADD_COLOR_CASE(TYPE, COLOR)\
	case TYPE: {\
	if (function_selected == *item) {\
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(COLOR.r, COLOR.g, COLOR.b, 1.0F));\
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(COLOR.r, COLOR.g, COLOR.b, 0.9F));\
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(COLOR.r, COLOR.g, COLOR.b, 1.0F));\
	}\
	else {\
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(COLOR.r, COLOR.g, COLOR.b, 0.8F));\
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(COLOR.r, COLOR.g, COLOR.b, 0.9F));\
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(COLOR.r, COLOR.g, COLOR.b, 1.0F));\
	}\
	break; }

void PanelDetailedFrame::ShowFunctions(std::list<Function*>* functions, float cursorX, float totalSize, double sizeMs, float maxX)
{
	ImGui::SetCursorPosX(cursorX);
	for (auto item = functions->begin(); item != functions->end(); ++item) {
		float itemSize = (((totalSize * (float)(*item)->ms) / (float)sizeMs));

		ImVec2 beforeCursor = ImGui::GetCursorPos();

		bool wasSelected = function_selected == *item;

		switch ((*item)->color) {
		ADD_COLOR_CASE(AsteroidColor::MAGENTA, Color::magenta);
		ADD_COLOR_CASE(AsteroidColor::BLACK, Color::black);
		ADD_COLOR_CASE(AsteroidColor::BLUE, Color::blue);
		ADD_COLOR_CASE(AsteroidColor::CYAN, Color::cyan);
		ADD_COLOR_CASE(AsteroidColor::GRAY, Color::gray);
		ADD_COLOR_CASE(AsteroidColor::GREEN, Color::green);
		ADD_COLOR_CASE(AsteroidColor::RED, Color::red);
		ADD_COLOR_CASE(AsteroidColor::WHITE, Color::white);
		ADD_COLOR_CASE(AsteroidColor::YELLOW, Color::yellow);
		ADD_COLOR_CASE(AsteroidColor::ORANGE, Color::orange);
		ADD_COLOR_CASE(AsteroidColor::PURPLE, Color::purple);
		default: {
			if (function_selected == *item) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive, 1.0F));
			}
			break; }
		}

		ImGui::PushID(*item);
		if (ImGui::Button(App->profile->functionNames[(*item)->nameIndex].data(), ImVec2(itemSize, 0))) {
			if (function_selected == *item) {
				ImGui::PopStyleColor((*item)->color != AsteroidColor::NONE ? 3 : 1);
			}
			App->ui->OnFunctionSelected(*item);
		}
		else if (function_selected == *item) {
			ImGui::PopStyleColor((*item)->color != AsteroidColor::NONE ? 3 : 1);
		}
		ImGui::PopID();
		if ((*item)->color != AsteroidColor::NONE && !wasSelected) {
			ImGui::PopStyleColor(3);
		}

		if (!(*item)->functions.empty()) {
			ShowFunctions(&(*item)->functions, beforeCursor.x, itemSize, (*item)->ms, beforeCursor.x + ImGui::GetItemRectSize().x);
		}
	
		if (*item != functions->back()) {
			float x = beforeCursor.x + itemSize + 1.0F;
			ImGui::SetCursorPos(ImVec2(min(x, maxX), beforeCursor.y));
		}
	}
}
