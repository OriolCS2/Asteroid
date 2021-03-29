#include "PanelSectionFunctions.h"
#include "imgui/imgui_internal.h"
#include "ModuleUI.h"
#include "PanelFrames.h"
#include "Frame.h"
#include "Function.h"

PanelSectionFunctions::PanelSectionFunctions(const std::string& panel_name) : Panel(panel_name)
{
}

PanelSectionFunctions::~PanelSectionFunctions()
{
}

void PanelSectionFunctions::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	Frame* frame = App->ui->panel_frames->frame;
	if (frame != nullptr) {
		
		auto& functions = frame->GetFunctionsData();

		ImGui::Columns(7, nullptr, false);

		ImGui::Text("Function");
		ImGui::SeparatorEx(ImGuiSeparatorFlags_SpanAllColumns | ImGuiSeparatorFlags_Vertical);
		ImGui::NextColumn();

		ImGui::Text("Total ms");
		ImGui::NextColumn();

		ImGui::Text("Max Call ms");
		ImGui::NextColumn();

		ImGui::Text("Average ms");
		ImGui::NextColumn();

		ImGui::Text("Total calls");
		ImGui::NextColumn();

		ImGui::Text("File");
		ImGui::NextColumn();

		ImGui::Text("Line");
		ImGui::SeparatorEx(ImGuiSeparatorFlags_SpanAllColumns | ImGuiSeparatorFlags_Horizontal);
		ImGui::NextColumn();

		for (auto item = functions.begin(); item != functions.end(); ++item) {
			ImGui::Text((*item)->name.data());
			ImGui::NextColumn();

			ImGui::Text("40");
			ImGui::NextColumn();

			ImGui::Text("40");
			ImGui::NextColumn();

			ImGui::Text("40");
			ImGui::NextColumn();

			ImGui::Text("40");
			ImGui::NextColumn();

			ImGui::Text((*item)->file.data());
			ImGui::NextColumn();

			ImGui::Text(std::to_string((*item)->line).data());
			ImGui::NextColumn();
		}
	}

	ImGui::End();
}