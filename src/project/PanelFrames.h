#pragma once

#include "Panel.h"

class Frame;

class PanelFrames : public Panel {
public:
	PanelFrames(const std::string& panel_name);
	virtual ~PanelFrames();

	void PanelLogic();

public:

	Frame* frame = nullptr;
};