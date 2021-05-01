#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "Panel.h"
#include "Time.h"
#include "MathGeoLib/include/Math/float3.h"
#include <utility>
#include <map>
#include <vector>

typedef unsigned long long u64;

class Panel;
class PanelFrames;
class ImFont;
class PanelSectionTree;
class PanelDetailedFrame;
class PanelSectionFunctions;

#define MAX_ON_PLAY_INI_PATH "Library/tmp.ini"

class Frame;
class Function;

enum class Font {
	DEFAULT,
	DEFAULT_X0F3,
	ICONS_17,
};

class ModuleUI : public Module
{
public:
	ModuleUI(bool start_enabled = true);
	~ModuleUI();

	bool Init();

	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();

	// draw UI
	void MainMenuBar();
	void Draw();

	void NormalizePath(std::string& path);

	void ChangeEnableDemo();

	ImFont* GetFont(Font font);

	void OnFrameDeselected();

	void OnFrameSelected(Frame* frame);
	void OnFunctionSelected(Function* function);

private:

	void LoadAsteroid();
	void LoadAsteroidBinary();
	void SaveAsteroid();
	void SaveAsteroidBinary();

	void UpdatePanels();

	void LoadFontDefaultFontWithIcons(float fontSize, const char* iconsPath, float iconsSize, ImWchar* fontRanges, Font font);
	void LoadFont(const char* path, float fontSize, Font font, const char* iconsPath = nullptr, float iconsSize = 0.0F, ImWchar* fontRanges = nullptr);
	void LoadIconsFont(const char* path, float iconsSIze, ImWchar* fontRanges, Font font);

private:
	// demo
	bool show_demo_window = false;
	bool showAbout = false;
	
	std::map<Font, ImFont*> imFonts;
	SDL_GLContext context;
public:
	std::vector<Panel*> panels;
	PanelFrames* panel_frames = nullptr;
	PanelDetailedFrame* panel_detailed_frame = nullptr;
	PanelSectionFunctions* panel_section_functions = nullptr;
	PanelSectionTree* panel_section_tree = nullptr;

	bool pauseImGui = false;
};