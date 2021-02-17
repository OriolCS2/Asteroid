#include "PanelSectionTree.h"
#include "ModuleUI.h"
#include "PanelDetailedFrame.h"
#include "Function.h"

PanelSectionTree::PanelSectionTree(const std::string& panel_name) : Panel(panel_name)
{
}

PanelSectionTree::~PanelSectionTree()
{
}

void PanelSectionTree::PanelLogic()
{
	ImGui::Begin(panel_name.data(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

	Function* function = App->ui->panel_detailed_frame->function_selected;
	if (function != nullptr) {
		ShowFunctionTree(function);
	}

	ImGui::End();
}

void PanelSectionTree::ShowFunctionTree(Function* function)
{
	static float itemBoxSpacingX = 23;
	static float boxSize = 130;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2.5F));
	ImGui::AlignTextToFramePadding();
	ImGui::PopStyleVar();

	ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
	bool isLeaf = function->functions.empty();

	ImGui::GetWindowDrawList()->AddRectFilled(cursorScreenPos + ImVec2(itemBoxSpacingX, 0), cursorScreenPos + ImVec2(boxSize + itemBoxSpacingX, 18), ImGui::GetColorU32(ImVec4(1, 0, 0, 1)));

	ImGui::Text(("    " + std::to_string(function->ms)).data());
	ImGui::SameLine();
	ImGui::Text(("            " + std::to_string(function->GetSelfMs())).data());
	ImGui::SameLine();

	ImGui::SetCursorScreenPos(cursorScreenPos);
	if (ImGui::TreeNodeEx(("                    " + function->name).data(), ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | (isLeaf ? ImGuiTreeNodeFlags_Leaf : 0))) {
		for (auto item = function->functions.begin(); item != function->functions.end(); ++item) {
			ShowFunctionTree(*item);
		}
		ImGui::TreePop();
	}
}
