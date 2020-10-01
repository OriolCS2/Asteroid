#pragma once

#include "Panel.h"

class PanelSectionTree : public Panel {
public:
	PanelSectionTree(const std::string& panel_name);
	virtual ~PanelSectionTree();

	void PanelLogic();
};