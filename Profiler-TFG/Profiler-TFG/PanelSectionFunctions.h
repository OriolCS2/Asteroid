#pragma once

#include "Panel.h"

class PanelSectionFunctions: public Panel {
public:
	PanelSectionFunctions(const std::string& panel_name);
	virtual ~PanelSectionFunctions();

	void PanelLogic();
};
