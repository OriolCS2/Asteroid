#include "ModuleUI.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "Application.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_opengl3.h"
#include <gl/GL.h>
#include "Time.h"
#include "SDL/include/SDL_assert.h"
#include "PanelDetailedFrame.h"
#include "PanelSectionTree.h"
#include "ModuleWindow.h"
#include "JSONfilepack.h"
#include "ModuleRenderer3D.h"
#include <string>
#include "PanelSectionFunctions.h"
#include "PanelFrames.h"
#include <fstream>
#include "IconFontCppHeaders/IconsFontAwesome5.h"
#include "mmgr/mmgr.h"
#include <ctime>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/glew32.lib") 

ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
	name.assign("UI");
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	return true;
}

bool ModuleUI::Start()
{
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

	static ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

	// load the default the first to set it as the main font, so no need to use PushFont and Pop font
	LoadFontDefaultFontWithIcons(13, "fa-solid-900.ttf", 12, icons_ranges, Font::DEFAULT);
	LoadFontDefaultFontWithIcons(16.9F, "fa-solid-900.ttf", 15.6F, icons_ranges, Font::DEFAULT_X0F3);
	LoadIconsFont( "fa-solid-900.ttf", 17, icons_ranges, Font::ICONS_17);

	ImGui::StyleColorsDark();

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);

	panels.push_back(panel_frames = new PanelFrames("Frames"));
	panels.push_back(panel_detailed_frame = new PanelDetailedFrame("Detailed Frame"));
	panels.push_back(panel_section_functions = new PanelSectionFunctions("Section Functions"));
	panels.push_back(panel_section_tree = new PanelSectionTree("Section Tree"));

	return ret;
}

bool ModuleUI::CleanUp()
{
	std::vector<Panel*>::iterator item = panels.begin();
	for (; item != panels.end(); ++item) {
		delete* item;
		*item = nullptr;
	}
	panels.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

void ModuleUI::Draw() 
{
	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}
	MainMenuBar();
	
	// Create Docking
	// Seeting Docking to fit the window and preferences
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace BackGround", (bool*)0, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}
	ImGui::End();
	// Create Docking
	UpdatePanels();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleUI::MainMenuBar()
{
	ImGui::BeginMainMenuBar();
	
	if (ImGui::BeginMenu("File")) {

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("About")) {

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void ModuleUI::ChangeEnableDemo()
{
	show_demo_window = !show_demo_window;
}

void ModuleUI::UpdatePanels()
{
	std::vector<Panel*>::iterator item = panels.begin();
	for (; item != panels.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->IsEnabled()) {
				(*item)->PanelLogic();
			}
			else if (!(*item)->desactive_called) {
				(*item)->desactive_called = true;
				(*item)->OnPanelDesactive();
			}
		}
	}

	if (pauseImGui) {
		ImGui::OpenPopup("PauseImGUi");
		ImGui::SetNextWindowSize({ 1,1 });
		ImGui::PushStyleColor(ImGuiCol_PopupBg, 0);
		ImGui::PushStyleColor(ImGuiCol_Border, 0);
		ImGui::PushStyleColor(ImGuiCol_ModalWindowDarkening, ImVec4(0.8F, 0.8F, 0.8F, 0.15F));
		ImGui::BeginPopupModal("PauseImGUi", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
		ImGui::EndPopup();
		ImGui::PopStyleColor(3);
	}
}

ImFont* ModuleUI::GetFont(Font font)
{
	return imFonts[font];
}

void ModuleUI::LoadFontDefaultFontWithIcons(float fontSize, const char* iconsPath, float iconsSize, ImWchar* fontRanges, Font font)
{
	ImFontConfig default_config;
	default_config.OversampleH = default_config.OversampleV = 1;
	default_config.PixelSnapH = true;
	default_config.SizePixels = fontSize * 1.0f;
	ImFormatString(default_config.Name, IM_ARRAYSIZE(default_config.Name), "ProggyClean.ttf, %dpx", (int)default_config.SizePixels);
	default_config.EllipsisChar = (ImWchar)0x0085;
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	imFonts.emplace(font, io.Fonts->AddFontDefault(&default_config));

	ImFontConfig icons_config; 
	icons_config.MergeMode = true; 
	icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(iconsPath, iconsSize, &icons_config, fontRanges);
}
void ModuleUI::LoadFont(const char* path, float fontSize, Font font, const char* iconsPath, float iconsSize, ImWchar* fontRanges)
{
	ImFontConfig font_config;
	font_config.OversampleH = font_config.OversampleV = 1;
	font_config.PixelSnapH = true;
	font_config.SizePixels = fontSize * 1.0f;
	strcpy(font_config.Name, path);
	font_config.EllipsisChar = (ImWchar)0x0085;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	imFonts.emplace(font, io.Fonts->AddFontFromFileTTF(path, fontSize, &font_config));

	if (iconsPath != nullptr) {
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF(iconsPath, iconsSize, &icons_config, fontRanges);
	}

}

void ModuleUI::LoadIconsFont(const char* path, float iconsSIze, ImWchar* fontRanges, Font font)
{
	ImFontConfig icons_config;
	icons_config.PixelSnapH = true;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	imFonts.emplace(font, io.Fonts->AddFontFromFileTTF(path, iconsSIze, &icons_config, fontRanges));
}