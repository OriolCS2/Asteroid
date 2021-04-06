#include "Panel.h"

#include "ModuleInput.h"
#include "imgui/imgui_internal.h"

Panel::Panel(const std::string& panel_name)
{
	this->panel_name = panel_name;
}

Panel::~Panel()
{
}

const std::string& Panel::GetName()
{
	return panel_name;
}

void Panel::ChangeEnable()
{
	enabled = !enabled;

	if (!enabled) {
		desactive_called = true;
		OnPanelDesactive();
	}
	else if (CanActive()) {
		OnPanelActive();
		desactive_called = false;
	}
	else {
		enabled = false;
	}
}

bool Panel::IsEnabled()
{
	return enabled;
}

void Panel::SetEnable(bool enabled)
{
	this->enabled = enabled;

	if (!this->enabled) {
		desactive_called = true;
		OnPanelDesactive();
	}
	else if (CanActive()) {
		OnPanelActive();
		desactive_called = false;
	}
	else {
		this->enabled = false;
	}
}

bool Panel::CanActive()
{
	return ImGui::GetTopMostPopupModal() == nullptr;
}