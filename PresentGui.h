#pragma once
#include "stdafx.h"
#include "xorstr.h"

static wchar_t* s2wc(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

UCanvas* pCanvas;

namespace settings
{
	bool Memory = false;
	bool XboxController = false;
	bool Sticky = false;
	bool teamcheck = false;
	bool randomaim = false;
	bool dbnocheck = false;
	bool pSilent = false;
	bool FovCircle = false;
	float FovSlide = 120.f;

	bool Box = false;
	bool Skeleton = false;
	bool Name = false;
	bool deutschescrosshair = false;
	bool crosshair = true;
	bool Distance = false;
	bool Weapon = false;
	bool Ammo = false;
	bool TargetLine = false;
	bool LootESP = false;
	bool ChestESP = false;
	bool AmmoESP = false;
	bool VehicleESP = false;
	bool LlamaESP = false;

	bool NoSpread = true;
	bool Test1337 = false;
	bool ATest1337 = false;
	bool AATest1337 = false;
	bool SpinBot = false;
	bool NoBloom = false;
	bool NoReload = false;
	bool VehicleFly = false;
	bool InstantRevive = false;
	bool AimWhileJumping = false;
	bool RapidFire = false;
	bool ProjectileTeleport = false; //
	bool RocketTeleport = false; //
	bool CameraFov = false;
	float CamerFovSlider = 0.f;
}

static void drawFilledRect(Vector2 initial_pos, float w, float h, FLinearColor color)
{
	for (float i = 0.0f; i < h; i += 1.0f)
		pCanvas->K2_DrawLine(Vector2{ initial_pos.X, initial_pos.Y + i }, Vector2{ initial_pos.X + w, initial_pos.Y + i }, color, 1.0f);
}


float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}

float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}

static void drawRect(Vector2 initial_pos, float w, float h, FLinearColor color, float thickness = 1.f)
{
	pCanvas->K2_DrawLine(initial_pos, Vector2(initial_pos.X + w, initial_pos.Y), color, thickness);
	pCanvas->K2_DrawLine(initial_pos, Vector2(initial_pos.X, initial_pos.Y + h), color, thickness);
	pCanvas->K2_DrawLine(Vector2(initial_pos.X + w, initial_pos.Y), Vector2(initial_pos.X + w, initial_pos.Y + h), color, thickness);
	pCanvas->K2_DrawLine(Vector2(initial_pos.X, initial_pos.Y + h), Vector2(initial_pos.X + w, initial_pos.Y + h), color, thickness);
}

namespace menu_framework {
	void group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, const wchar_t* string, bool show_label);
	void tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, const wchar_t* string, std::int32_t& tab, std::int32_t count, bool show_outline);
	void check_box(std::int32_t x, std::int32_t y, std::int32_t position, const wchar_t* string, bool& value);
	void slider(std::int32_t x, std::int32_t y, std::int32_t position, const std::string& string, float& value, float min_value, float max_value);
	void menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);

	inline bool should_drag = false;
	inline bool should_move = false;
};

POINT cursor;
POINT cursor_corrected;

void menu_framework::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, const wchar_t* string, bool show_label) {
	drawFilledRect(Vector2(x, y), w, h, FLinearColor(0.004060814f, 0.004060814f, 0.004060814f, 1.000000000f));
	drawRect(Vector2(x, y), w, h, FLinearColor(0.005060814f, 0.005060814f, 0.005060814f, 1.000000000f));
	if (show_label)
		pCanvas->K2_DrawText(Vector2(x + 2, y - 12), string, HackColors::White, false, false);
}

void menu_framework::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, const wchar_t* string, std::int32_t& tab, std::int32_t count, bool show_outline) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
		tab = count;

	drawRect(Vector2(x, y), w, h, tab == count ? HackColors::SlateGray : HackColors::LimeGreen);

	pCanvas->K2_DrawText({ (float)x - 50 / 2 + 50,(float)y + h / 2 - 8 }, string, tab == count ? HackColors::White : HackColors::LimeGreen, false, false);
}

void menu_framework::check_box(std::int32_t x, std::int32_t y, std::int32_t position, const wchar_t* string, bool& value) {
	GetCursorPos(&cursor);

	int w = 10, h = 10;

	if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1)
		value = !value;

	drawFilledRect(Vector2(position, y), w, h, value ? HackColors::LimeGreen : FLinearColor(0.008060814f, 0.008060814f, 0.008060814f, 1.000000000f));
	pCanvas->K2_DrawText(Vector2(x + 2, y - 1), string, HackColors::White, false, false);
}

void menu_framework::slider(std::int32_t x, std::int32_t y, std::int32_t position, const std::string& string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);

	int ix = x + 140;
	int yi = y + 4;

	if ((cursor.x > ix) && (cursor.x < ix + position) && (cursor.y > yi) && (cursor.y < yi + 6) && (GetAsyncKeyState(VK_LBUTTON)))
		value = (cursor.x - ix) / (float(position) / float(max_value));

	drawFilledRect(Vector2(ix, yi), position, 6, FLinearColor(0.008060814f, 0.008060814f, 0.008060814f, 1.000000000f));
	drawFilledRect(Vector2(ix, yi), value * (float(position) / float(max_value)), 6, HackColors::LimeGreen);

	pCanvas->K2_DrawText(Vector2(x + 2, y - 1), s2wc((std::stringstream{ } << string << ": " << std::setprecision(3) << value).str().c_str()), HackColors::White, false, false);
}

void menu_framework::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}

	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}

	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}

namespace variables {
	namespace menu {
		inline bool opened = false;
		inline int x = 140, y = 140;
		inline int w = 400, h = 300;
	}
}

Vector2 CursorPos()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	return Vector2{ (float)cursorPos.x, (float)cursorPos.y };
}

static void Draw_Cursor(bool toogle)
{
	if (toogle)
	{
		Vector2 cursorPos = CursorPos();
		drawFilledRect(Vector2(cursorPos.X, cursorPos.Y), 6.f, 6.f, HackColors::Cyan);
	}
}

#pragma once

class dot {
public:
	dot(Vector2 p, Vector2 v) {
		m_vel = v;
		m_pos = p;
	}

	void update();
	void draw();

	Vector2 m_pos, m_vel;
};

std::vector<dot*> dots = { };

void dot::update() {
	m_pos += m_vel;
}

void dot::draw() {
	drawFilledRect(Vector2(m_pos.X - 2, m_pos.Y- 2), 5, 5, HackColors::White);

	auto t = std::find(dots.begin(), dots.end(), this);
	if (t == dots.end()) {
		return;
	}

	for (auto i = t; i != dots.end(); i++) {
		if ((*i) == this) continue;

		auto dist = (m_pos - (*i)->m_pos).Length();

		if (dist < 128) {
			pCanvas->K2_DrawLine(Vector2(m_pos.X - 1, m_pos.Y - 2), Vector2((*i)->m_pos.X - 2, (*i)->m_pos.Y - 1), HackColors::White, 1.f);
		}
	}
}

// micca is hot

void dot_draw() {
	struct screen_size {
		int x, y;
	}; screen_size sc;

	sc.x = GetSystemMetrics(0);
	sc.y = GetSystemMetrics(1);

	int s = rand() % 5;

	if (s == 0) {
		dots.push_back(new dot(Vector2(rand() % (int)sc.x, -10), Vector2((rand() % 1) - 3, rand() % 3 + 1)));
	}
	else if (s == 1) {
		dots.push_back(new dot(Vector2(rand() % (int)sc.x, (int)sc.y + 10), Vector2((rand() % 1) - 3, -1 * (rand() % 3 + 1))));
	}
	else if (s == 2) {
		dots.push_back(new dot(Vector2(-10, rand() % (int)sc.y), Vector2(rand() % 3 + 1, (rand() % 1) - 3)));
	}
	else if (s == 3) {
		dots.push_back(new dot(Vector2((int)sc.x + 10, rand() % (int)sc.y), Vector2(-1 * (rand() % 1 + 1), (rand() % 1) - 3)));
	}

	for (auto i = dots.begin(); i < dots.end();) {
		if ((*i)->m_pos.Y < -20 || (*i)->m_pos.Y > sc.y + 20 || (*i)->m_pos.X < -20 || (*i)->m_pos.X > sc.x + 20) {
			delete (*i);
			i = dots.erase(i);
		}
		else {
			(*i)->update();
			i++;
		}
	}

	for (auto i = dots.begin(); i < dots.end(); i++) {
		(*i)->draw();
	}
}

void dot_clear()
{
	for (auto i = dots.begin(); i < dots.end(); i++) {
		delete (*i);
	}

	dots.clear();
}

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, FLinearColor bg, FLinearColor header_text, FLinearColor header_line, const std::string& name) {
	//dot_draw();
	drawFilledRect(Vector2(x, y), w, h, bg);
	drawFilledRect(Vector2(x, y), w, 30, header_text);
	drawFilledRect(Vector2(x, y + 30), w, 2, header_line);
	pCanvas->K2_DrawText(Vector2(x + 10, y + 8), s2wc(name.c_str()), HackColors::White, false, false);
};

void DrawGUI(UCanvas* canvas, bool menu_g)
{
	pCanvas = canvas;

	static int tab = 0;
	if (menu_g)
	{
		std::string nignog = xorstr("MEAX FORTNITE (Built: " __TIME__ " " __DATE__ ")");
		do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, FLinearColor(0.005060814f, 0.005060814f, 0.005060814f, 1.000000000f), FLinearColor(0.004060814f, 0.004060814f, 0.004060814f, 1.000000000f), FLinearColor(0.005060814f, 0.005060814f, 0.005060814f, 1.000000000f), nignog.c_str());

		menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, 100, 260, L"I LIKE BALLS", false); {
			menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) - 35, 100, 30, L"Aimbot", tab, 0, false);
			menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2), 100, 30, L"Visuals", tab, 1, false);
			menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) + 35, 100, 30, L"Exploits", tab, 2, false);
			menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) + 705, 100, 30, L"Misc", tab, 3, false);
		}

		switch (tab) {
		case 0:
			menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, L"aimbot", false); {
				pCanvas->K2_DrawText(Vector2(variables::menu::x + 122, variables::menu::y + 45), L"Legit Option:", HackColors::White, false, false);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, L"Memory Aimbot", settings::Memory);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, L"Aimbot Fov Circle", settings::FovCircle);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 90, 125, "Fov Slider", settings::FovSlide, 0.f, 360.f);
				pCanvas->K2_DrawText(Vector2(variables::menu::x + 122, variables::menu::y + 110), L"Risk Option:", HackColors::White, false, false);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 125, variables::menu::x + 375, L"Silent Aimbot", settings::pSilent);
			}
			break;
		case 1:
			menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, L"visuals", false); {
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, L"Box", settings::Box);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, L"Skeleton", settings::Skeleton);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, L"Name", settings::Name);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, L"Distance", settings::Distance);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, variables::menu::x + 375, L"Weapon", settings::Weapon);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 120, variables::menu::x + 375, L"Ammo", settings::Ammo);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 150, variables::menu::x + 375, L"Vehicle and Boats", settings::VehicleESP);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 165, variables::menu::x + 375, L"Chest", settings::ChestESP);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 180, variables::menu::x + 375, L"Ammo Box", settings::AmmoESP);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 195, variables::menu::x + 375, L"Supplies", settings::LlamaESP);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 210, variables::menu::x + 375, L"Loot and Weapons", settings::LootESP);
			}
			break;
		case 2:
			menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, L"exploits", false); {
				pCanvas->K2_DrawText(Vector2(variables::menu::x + 122, variables::menu::y + 45), L"Very Risky:", HackColors::OrangeRed, false, false);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, L"Chams", settings::AATest1337);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, L"No Reload", settings::NoReload);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, L"No Ads", settings::AimWhileJumping);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, variables::menu::x + 375, L"No Revive", settings::InstantRevive);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 120, variables::menu::x + 375, L"Rapid Fire", settings::RapidFire);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 135, variables::menu::x + 375, L"Projectile Teleport", settings::ProjectileTeleport);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 150, variables::menu::x + 375, L"Camera FOV Changer", settings::CameraFov);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 165, 125, "Camera FOV Slider", settings::CamerFovSlider, 0.f, 120.f);
			}
			break;
		case 3:
			menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, L"misc", false); {
				pCanvas->K2_DrawText(Vector2(variables::menu::x + 120, variables::menu::y + 45), L"", HackColors::White, false, false);
				pCanvas->K2_DrawText(Vector2(variables::menu::x + 120, variables::menu::y + 45), L"https://discord.gg/FQW36pnENf", HackColors::White, false, false);
			}
			break;
		}

	}
	else
	{
		//dot_clear();
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
	Draw_Cursor(menu_g);

	static char memes[128];
	static wchar_t wmemes[128];
	sprintf(memes, ("FPS: %d"), GetFPSForHUD(GController));
	AnsiToWide(memes, wmemes);
	pCanvas->K2_DrawText(Vector2(50, 50), wmemes, HackColors::Red, false, false);
}


/*BOOL DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE;
	CreateConsole();
	LoadSignatures();
	LoadObjects();
	auto B = utils::PatternScan(xorstr("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 D2 48 8D 05 ? ? ? ? 48 89 03 48 8B C3")) + 16;
	B = RVA(B, 7);
	VirtualTableFunctionSwap((void*)B, ProcessEventHooked, (void**)&ProcessEventOriginal, 0x220);
	B = B - 0x118;
	VirtualTableFunctionSwap((void*)B, ProcessEventHooked, (void**)&ProcessEventOriginal, 0x220);
	auto GWorld = *(uintptr_t*)(Addresses::UWorld);
	auto World = (UWorld*)GWorld;
	auto ad = World->OwningGameInstance()->LocalPlayers()->LocalPlayer()->PlayerController()->MyHud();
	printf("HUD OBJECT VTABLE: 0x%llx\n", *(uint64_t*)ad - (uint64_t)GetModuleHandleA(0));
	return TRUE;
}*/