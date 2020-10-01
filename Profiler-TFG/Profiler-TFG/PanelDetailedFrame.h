#pragma once

#include "Panel.h"

class PanelDetailedFrame : public Panel {
public:
	PanelDetailedFrame(const std::string& panel_name);
	virtual ~PanelDetailedFrame();

	void PanelLogic();
};
