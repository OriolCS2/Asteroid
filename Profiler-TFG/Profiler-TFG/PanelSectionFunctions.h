#pragma once

#include "Panel.h"

class Frame;

class PanelSectionFunctions: public Panel {
public:
	PanelSectionFunctions(const std::string& panel_name);
	virtual ~PanelSectionFunctions();

	void PanelLogic();

};
