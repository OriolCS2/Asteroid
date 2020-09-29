#pragma once

#include "Globals.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL.h"
#include <string>
#include <vector>
#include "Application.h"
#include "MathGeoLib/include/Math/float3.h"

class Panel {
	friend class ModuleUI;
public:

	Panel(const std::string& panel_name);
	virtual ~Panel();

	virtual void PanelLogic() {};

	const std::string& GetName();
	void ChangeEnable();
	bool IsEnabled();
	void SetEnable(bool enabled);

protected:
	static bool CanActive();

private:

	virtual void OnPanelDesactive() {};
	virtual void OnPanelActive() {};

protected:
	std::string panel_name;
	bool enabled = false;

private: 

	bool desactive_called = false;
};