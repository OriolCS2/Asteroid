#pragma once

#include "Panel.h"

class Frame;
class Function;

class PanelDetailedFrame : public Panel {
public:
	PanelDetailedFrame(const std::string& panel_name);
	virtual ~PanelDetailedFrame();

	void PanelLogic();

private:

	void ShowFunctions(std::list<Function*>* functions, float cursorX, float totalSize, double sizeMs, float maxX);

public:

	Function* function_selected = nullptr;

private:

	float scale = 1.0F;

};
