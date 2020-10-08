#include "PanelFrames.h"
#include "Frame.h"
#include "Application.h"
#include "ModuleProfile.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#define FRAME_WIDTH 25

PanelFrames::PanelFrames(const std::string& panel_name) : Panel(panel_name)
{

}

PanelFrames::~PanelFrames()
{
}

static void AddTextVertical(ImDrawList* DrawList, const char* text, ImVec2 pos, ImU32 text_color) {
	pos.x = int(pos.x);
	pos.y = int(pos.y);
	ImFont* font = GImGui->Font;
	const ImFontGlyph* glyph;
	char c;
	ImGuiContext& g = *GImGui;
	ImVec2 text_size = ImGui::CalcTextSize(text);
	while ((c = *text++)) {
		glyph = font->FindGlyph(c);
		if (!glyph) continue;

		DrawList->PrimReserve(6, 4);
		DrawList->PrimQuadUV(
			pos + ImVec2(glyph->Y0, -glyph->X0),
			pos + ImVec2(glyph->Y0, -glyph->X1),
			pos + ImVec2(glyph->Y1, -glyph->X1),
			pos + ImVec2(glyph->Y1, -glyph->X0),

			ImVec2(glyph->U0, glyph->V0),
			ImVec2(glyph->U1, glyph->V0),
			ImVec2(glyph->U1, glyph->V1),
			ImVec2(glyph->U0, glyph->V1),
			text_color);
		pos.y -= glyph->AdvanceX;
	}
}

static float Saw(void*, int i) 
{	
	int index = 0;
	for (auto item = App->profile->frames.begin(); item != App->profile->frames.end(); ++item) {
		if (index == i) {
			return (*item)->ms;
		}
		++index;
	}
	return 0;
}

void PanelFrames::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 0));
	for (auto item = App->profile->frames.begin(); item != App->profile->frames.end(); ++item) {
		ImGui::PushID(*item);
		ImGui::Button("##", { FRAME_WIDTH, (float)(*item)->ms });
		ImGui::PopID();
		std::string ms_text = std::to_string((*item)->ms) + " ms";
		AddTextVertical(ImGui::GetWindowDrawList(), ms_text.data(), ImGui::GetItemRectMin() + ImVec2(7, ImGui::CalcTextSize(ms_text.data()).x + 5), ImGui::GetColorU32(ImGuiCol_Text));
		ImGui::SameLine();
	}
	ImGui::PopStyleVar();

	ImVec2 minLinesScreen = ImGui::GetWindowContentRegionMin() + ImVec2(0, ImGui::GetWindowPos().y);
	ImVec2 maxLinesScreen = ImGui::GetWindowContentRegionMax() + ImVec2(0, ImGui::GetWindowPos().y);

	float windowHeight = maxLinesScreen.y - minLinesScreen.y;
	float windowOffset = minLinesScreen.y;

	ImGui::GetWindowDrawList()->AddLine({ 0, minLinesScreen.y }, { ImGui::GetWindowWidth(), minLinesScreen.y }, ImGui::GetColorU32(ImGuiCol_Separator));
	
	float posY = windowHeight * 0.25F;
	ImGui::SetCursorScreenPos({ 7, windowOffset + posY - ImGui::GetFont()->FontSize - 3});
	ImGui::Text("16.6 ms");
	ImGui::GetWindowDrawList()->AddLine({ 0, posY + windowOffset }, { ImGui::GetWindowWidth(), posY + windowOffset }, ImGui::GetColorU32(ImGuiCol_Separator));

	posY = windowHeight * 0.5F;
	ImGui::SetCursorScreenPos({ 7, windowOffset + posY - ImGui::GetFont()->FontSize - 3});
	ImGui::Text("33.3 ms");
	ImGui::GetWindowDrawList()->AddLine({ 0, posY + windowOffset }, { ImGui::GetWindowWidth(), posY + windowOffset }, ImGui::GetColorU32(ImGuiCol_Separator));

	posY = windowHeight * 0.75F;
	ImGui::SetCursorScreenPos({ 7, windowOffset + posY - ImGui::GetFont()->FontSize - 3});
	ImGui::Text("50 ms");
	ImGui::GetWindowDrawList()->AddLine({ 0, posY + windowOffset }, { ImGui::GetWindowWidth(), posY + windowOffset }, ImGui::GetColorU32(ImGuiCol_Separator));

	posY = windowHeight;
	ImGui::SetCursorScreenPos({ 7, windowOffset + posY - ImGui::GetFont()->FontSize - 3});
	ImGui::Text("66.6 ms");
	ImGui::GetWindowDrawList()->AddLine({ 0, posY + windowOffset }, { ImGui::GetWindowWidth(), posY + windowOffset }, ImGui::GetColorU32(ImGuiCol_Separator));

	ImGui::End();
}
