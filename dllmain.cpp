#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <string>
#include "xorstr.h"
#include "utils.h"
#include "GameSDK.h"
#include "PresentGui.h"
#include "Controller.h"
#include "Kernelcontrol.h"
#include <intrin.h>

bool ShowMenu = false;


uintptr_t GPawn;
UCanvas* ChainFunc;

PVOID(*GetWeaponStats)(PVOID) = nullptr;

void DrawBones(UObject* mesh, Bones* arr, int sz, FLinearColor clr, Vector2& topleft, Vector2& downright, float b_thicc = 1.4f)
{
	for (int i = 1; i < sz; i++)
	{
		Vector2 spPrev;
		Vector2 spNext;

		Vector3 previousBone = GetBone3D(mesh, arr[i - 1]);

		if (previousBone == Vector3())
			continue;

		if (!W2S(previousBone, spPrev))
			continue;

		Vector3 nextBone = GetBone3D(mesh, arr[i]);

		if (nextBone == Vector3())
			continue;

		if (!W2S(nextBone, spNext))
			continue;

		if (previousBone.DistanceFrom(nextBone) > 100)
			continue;

		auto x = spPrev;

		if (x.X > downright.X)
			downright.X = x.X;

		if (x.Y > downright.Y)
			downright.Y = x.Y;

		if (x.X < topleft.X)
			topleft.X = x.X;

		if (x.Y < topleft.Y)
			topleft.Y = x.Y;

		x = spNext;

		if (x.X > downright.X)
			downright.X = x.X;

		if (x.Y > downright.Y)
			downright.Y = x.Y;

		if (x.X < topleft.X)
			topleft.X = x.X;

		if (x.Y < topleft.Y)
			topleft.Y = x.Y;

		if (settings::Skeleton)
		{
			ChainFunc->K2_DrawLine(spPrev, spNext, clr, b_thicc);
		}
			
	}
}

void DrawBox(Vector2& topleft, Vector2& downright, FLinearColor clr)
{
	auto h = downright.Y - topleft.Y;
	auto w = downright.X - topleft.X;

	auto downleft = Vector2{ topleft.X, downright.Y };
	auto topright = Vector2{ downright.X, topleft.Y };

	ChainFunc->K2_DrawLine(topleft, { topleft.X, topleft.Y + h * 0.45f }, clr, 1.f);
	ChainFunc->K2_DrawLine(topleft, { topleft.X + w * 0.45f, topleft.Y }, clr, 1.f);

	ChainFunc->K2_DrawLine(downright, { downright.X, downright.Y - h * 0.45f }, clr, 1.f);
	ChainFunc->K2_DrawLine(downright, { downright.X - w * 0.45f, downright.Y }, clr, 1.f);

	ChainFunc->K2_DrawLine(downleft, { downleft.X, downleft.Y - h * 0.45f }, clr, 1.f);
	ChainFunc->K2_DrawLine(downleft, { downleft.X + w * 0.45f, downright.Y }, clr, 1.f);

	ChainFunc->K2_DrawLine(topright, { topright.X, topright.Y + h * 0.45f }, clr, 1.f);
	ChainFunc->K2_DrawLine(topright, { topright.X - w * 0.45f, topright.Y }, clr, 1.f);
}

float namePosX;
float namePosY;
Vector3 aim;

typedef signed int(__thiscall* tPostRender) (UObject*, UCanvas*);
tPostRender oPostRender = NULL;
signed int __stdcall hPostRender(UObject* pCallObject, UCanvas* pUFunc)
{
	ChainFunc = pUFunc;
	BULLETTELEPORT = pUFunc;

	if (GetAsyncKeyState(VK_INSERT) & 1)
		ShowMenu = !ShowMenu;

	do {
		float olddistance = 0x7FFF;
		auto UWorld = *(uintptr_t*)(AUWorld); if (!UWorld) return false;
		auto GameInstance = *(uintptr_t*)(UWorld + 0x190);
		auto LocalPlayers = *(uintptr_t*)(GameInstance + 0x38);
		auto ULocalPlayer = *(uintptr_t*)(LocalPlayers);
		GController = *(uintptr_t*)(ULocalPlayer + 0x30);
		auto GPlayerCameraManager = *(uintptr_t*)(GController + 0x2C8);
		if (GPlayerCameraManager)
		{
			GPawn = *(uintptr_t*)(GController + 0x2B0);
			float FovAngle = APlayerCameraManager::GetFOVAngle(GPlayerCameraManager);
			APlayerCameraManager::GetPlayerViewPoint(GController, &CamLoc, &CamRot);
			auto Levels = *(TArray<UObject*>*)((uintptr_t)UWorld + 0x148); if (!Levels.Num()) return false;
			for (int levelIndex = 0; (true ? (levelIndex != Levels.Num()) : levelIndex != 1); levelIndex++)
			{
				auto Level = Levels[levelIndex];
				auto ActorArray = (TArray<UObject*>*)((uintptr_t)Level + 0x98);
				auto actors = *ActorArray;
				for (int i = 0; i < actors.Num(); i++)
				{
					auto actor = actors[i];

					if (!actor)
						continue;

					if (actor == (UObject*)GPawn)
						continue;

					auto ActorName = GetObjectName((uintptr_t)actor);

					if (levelIndex == 0 && strstr(ActorName, xorstr("PlayerPawn")))
					{
						if (actor == (UObject*)GPawn)
							continue;

						auto rc = *(UObject**)((uintptr_t)actor + 0x138);

						if (!rc)
							continue;

						Vector3 loc = *(Vector3*)((uintptr_t)rc + 0x11C);

						auto dsit = GetDistanceMeters(loc);

						Vector2 sp;

						FLinearColor Visible = HackColors::White;
						FLinearColor Distance = HackColors::Yellow;
						Vector3 ViewPoint = { 0,0,0 };
						bool IsVisible = APlayerCameraManager::LineOfSightTo((PVOID)GController, (PVOID)actor, &ViewPoint);
						if (IsVisible)
						{
							Visible = HackColors::Red; Distance = HackColors::White;
						}
						else if (!IsVisible)
						{
							Visible = HackColors::White; Distance = HackColors::Yellow;
						}

						if (W2S(loc, sp))
						{
							auto mesh = *(UObject**)((uintptr_t)actor + 0x288);

							if (!mesh)
								continue;

							static Bones p1[] =
							{
								Bones(56),
								Bones(94),
								Bones(93),
								Bones(37),
								Bones(9),
								Bones(10),
								Bones(27)
							};

							static Bones p2[] =
							{
								(Bones)68,
								(Bones)67,
								(Bones)2
							};

							static Bones p3[] =
							{
								Bones(81),
								Bones(85),
								Bones(77),
								Bones(76),
								Bones(2),
								Bones(69),
								Bones(70),
								Bones(84),
								Bones(74)
							};

							Vector2 downright;
							Vector2 topleft;
							float box_center_offset_up = 0;
							float box_center_offset_down = 0;

							if (settings::Skeleton || settings::Box)
							{
								topleft = sp;
								downright = sp;

								DrawBones(mesh, p1, sizeof(p1), Visible, topleft, downright, 1.f);
								DrawBones(mesh, p2, sizeof(p2), Visible, topleft, downright, 1.f);
								DrawBones(mesh, p3, sizeof(p3), Visible, topleft, downright, 1.f);

								if (point_valid(topleft) && point_valid(downright))
								{
									Vector2 dim(downright.X - topleft.X, downright.Y - topleft.Y);

									topleft -= dim * 0.10;
									downright += dim * 0.10;

									topleft.Y -= dim.Y * 0.10;
									downright.Y += dim.Y * 0.10;
		
									if (settings::Box)
									{
										DrawBox(topleft, downright, Visible);
									}

									box_center_offset_up = ((downright.X - topleft.X) / 2);
									box_center_offset_down = ((topleft.X - downright.X) / 2);
								}
							}
							else
							{
								topleft = sp;
								downright = sp;
							}

							float offset = -24;

							if (settings::Name)
							{
								auto playerState = *(uintptr_t*)((uintptr_t)actor + 0x240);
								if (!playerState)
									continue;

								if (playerState)
								{
									auto name = GetPlayerName(playerState);
									if (name.c_str())
									{
										xDrawText(name.c_str(), topleft + Vector2(0, offset), HackColors::Red, box_center_offset_up);
										offset -= 14;
									}
								}
							}

							if (settings::Ammo)
							{
								auto weapon = *(uintptr_t*)((uintptr_t)actor + 0x5F8);
								if (!weapon)
									continue;
								if (weapon)
								{
									int actorAmmoCount = *(int*)(weapon + 0xA70);
									if (actorAmmoCount)
									{
										xDrawText(s2wc(TextFormat("%i / %i", actorAmmoCount, actorAmmoCount - 5).c_str()), topleft + Vector2(0, offset), HackColors::Yellow, box_center_offset_up);
									}
								}
								offset -= 14;
							}

							if (settings::Weapon)
							{
								auto weapon = *(uintptr_t*)((uintptr_t)actor + 0x5F8);
								if (!weapon)
									continue;
								if (weapon)
								{
									auto wdata = *(uintptr_t*)(weapon + 0x380);
									if (!wdata)
										continue;
									if (wdata)
									{
										auto name = *(FText*)(wdata + 0x88);
										auto tier = *(UINT8*)(wdata + 0x6C);
										auto naam = name.Get();
										if (naam && tier > 0)
										{
											if (naam[0] != 'H' && naam[1] != 'a')
											{
												auto color = HackColors::LightYellow;

												switch (tier)
												{
												case 0:
												case 1:
													break;
												case 2:
													color = HackColors::LightGreen;
													break;
												case 3:
													color = HackColors::DarkCyan;
													break;
												case 4:
													color = HackColors::Purple;
													break;
												case 5:
													color = HackColors::Orange;
													break;
												}

												xDrawText(naam, topleft + Vector2(0, offset), color, box_center_offset_up);
												offset -= 14;
											}
										}
									}
								}

							}

						/*	if (settings::aatest1337)
							{
								fpawnhighlight hitglow;
								hitglow.usespulse = 0;
								hitglow.inner = flinearcolor(60000.f, 0.f, 0.f, 255.f);
								hitglow.outer = flinearcolor(60000.f, 0.f, 0.f, 255.f);

								applypawnhighlight(actor, hitglow);
							}*/

							if (settings::Distance)
							{
								if (!actor)
									continue;
								static char memes[128];
								static wchar_t wmemes[128];
								sprintf(memes, ("%d m"), dsit);
								AnsiToWide(memes, wmemes);
								xDrawText(wmemes, topleft + Vector2(0, offset), Distance, box_center_offset_up); // dist
							}

							if (valid_pointer(GPawn))
							{
							

								Vector3 HeadBox;
								USkeletalMeshComponent::GetBoneLocation((uintptr_t)actor, 68, &HeadBox);
								AController::WorldToScreen(GController, HeadBox, &HeadBox);
								auto x = HeadBox.x - (GetSystemMetrics(0) / 2);
								auto y = HeadBox.y - (GetSystemMetrics(1) / 2);
								auto distance = sqrt(x * x + y * y);
								if (distance < settings::FovSlide && distance < olddistance)
								{
									USkeletalMeshComponent::GetBoneLocation((uintptr_t)actor, 68, &aim);
									olddistance = distance;
								}
							}
						}
					}
					if (valid_pointer(GPawn))
					{
						if (settings::ProjectileTeleport)
						{
							if (levelIndex == 0 && strstr(ActorName, xorstr("Bullet"))) {
								UObject* ProjectileClass = (UObject*)(uintptr_t)actor;
								if (ProjectileClass)
								{
									FHitResult HitResultat;
									K2_SetActorLocation((uintptr_t)ProjectileClass, aim, false, true, &HitResultat);
								}
							}
						}
					}
				}
			}

			if (valid_pointer(GPawn))
			{
				if (settings::Memory)
				{
					if (settings::XboxController)
					{
						if (IsPressingRightTrigger())
						{
							auto angle = calc_angle(CamLoc, aim);
							SetViewAngles(pUFunc, angle, GController, GPawn);
						}
					}
					else if (GetAsyncKeyState(VK_RBUTTON))
					{
						auto angle = calc_angle(CamLoc, aim);
						SetViewAngles(pUFunc, angle, GController, GPawn);
					}
				}
				
				if (settings::AimWhileJumping)
				{
					*(bool*)(GPawn + 0x3E51) = true;
				}

				if (settings::InstantRevive)
				{
					*(float*)(GPawn + 0x3758) = 1;
				}

				if (settings::CameraFov)
				{
					FOV(GController, settings::CamerFovSlider);
				}

				if (settings::RapidFire)
				{
					float a = 0;
					float b = 0;
					uintptr_t CurrentWeapon = *(uintptr_t*)(GPawn + 0x5F8); //CurrentWeapon Offset
					if (!CurrentWeapon)
						continue;
					if (CurrentWeapon) {
						a = *(float*)(CurrentWeapon + 0x9EC); //LastFireTime Offset
						b = *(float*)(CurrentWeapon + 0x9F0); //LastFireTimeVerified Offset
						*(float*)(CurrentWeapon + 0x9EC) = a + b - 0.3333; //LastFireTime Offset
					}
				}
			}

			if (settings::FovCircle)
				DrawCircle(pUFunc, GetSystemMetrics(0) / 2, GetSystemMetrics(1) / 2, settings::FovSlide * GetSystemMetrics(0) / 2 / FovAngle / 2, 124.f, HackColors::Red);

			if (settings::crosshair)
			{
				DrawLine(pUFunc, GetSystemMetrics(0) / 2 - 10.f, GetSystemMetrics(1) / 2 - 0, GetSystemMetrics(0) / 2 + 10.f, GetSystemMetrics(1) / 2 + 0, 1.0f, HackColors::Red);
				DrawLine(pUFunc, GetSystemMetrics(0) / 2 + 0, GetSystemMetrics(1) / 2 - 10.f, GetSystemMetrics(0) / 2 - 0, GetSystemMetrics(1) / 2 + 10.f, 1.0f, HackColors::Red);
			}
		}

	} while (FALSE);

	DrawGUI((UCanvas*)pUFunc, ShowMenu);

	return oPostRender(pCallObject, pUFunc);
}

static float* (*CalculateShotOriginal)(uintptr_t, uintptr_t, uintptr_t) = nullptr;
static float* CalculateShotHook(uintptr_t arg0, uintptr_t arg1, uintptr_t arg2) {
	auto ret = CalculateShotOriginal(arg0, arg1, arg2);
	if (ret && GPawn) {
		if (settings::pSilent) {
			Vector3 TargetHead = aim;
			TargetHead.z = TargetHead.z - 16.0f;
			auto rc = *(UObject**)((uintptr_t)GPawn + 0x130);
			Vector3 LocalPawnRelativeLocation = *(Vector3*)((uintptr_t)rc + 0x11C);

			auto y_tmp = atan2f(TargetHead.y - LocalPawnRelativeLocation.y, TargetHead.x - LocalPawnRelativeLocation.x);

			LocalPawnRelativeLocation.x = LocalPawnRelativeLocation.x + cosf(y_tmp + 1.5708f) * 32.0f;
			LocalPawnRelativeLocation.y = LocalPawnRelativeLocation.y + sinf(y_tmp + 1.5708f) * 32.0f;
			LocalPawnRelativeLocation.z = LocalPawnRelativeLocation.z + 45.0f;

			auto length = sqrtf(powf(TargetHead.x - LocalPawnRelativeLocation.x, 2.0f) + powf(TargetHead.y - LocalPawnRelativeLocation.y, 2.0f));
			Vector3 NewPos;

			NewPos.x = -atan2f(TargetHead.z - LocalPawnRelativeLocation.z, length) / 2.0f;
			NewPos.y = atan2f(TargetHead.y - LocalPawnRelativeLocation.y, TargetHead.x - LocalPawnRelativeLocation.x) / 2.0f;

			ret[0] = -(sinf(NewPos.x) * sinf(NewPos.y));
			ret[1] = sinf(NewPos.x) * cosf(NewPos.y);
			ret[2] = cosf(NewPos.x) * sinf(NewPos.y);
			ret[3] = cosf(NewPos.x) * cosf(NewPos.y);
			return ret;
		}
	}

	return ret;
}

VOID Main() {
	/*AllocConsole();
	static_cast<VOID>(freopen(xorstr("CONIN$"), xorstr("r"), stdin));
	static_cast<VOID>(freopen(xorstr("CONOUT$"), xorstr("w"), stdout));
	static_cast<VOID>(freopen(xorstr("CONOUT$"), xorstr("w"), stderr));*/

	ScreenCenterX = GetSystemMetrics(0);
	ScreenCenterY = GetSystemMetrics(1);
	//Depth = GetSystemMetrics(2);

	AUWorld = Utils::FindSiganture(xorstr("48 89 05 ? ? ? ? 48 8B 4B 78"));
	AUWorld = RELATIVE_ADDRESS(AUWorld, 7);
	FreeObject = Utils::FindSiganture(xorstr("48 85 C9 0F 84 ? ? ? ? 53 48 83 EC 20 48 89 7C 24 30 48 8B D9 48 8B 3D ? ? ? ? 48 85 FF 0F 84 ? ? ? ? 48 8B 07 4C 8B 40 30 48 8D 05 ? ? ? ? 4C 3B C0"));
	//48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B F2 4C 8B F1 E8 ? ? ? ? 45 8B 06 33 ED 41 0F B7 16 41 C1 E8 10 89 54 24 24 44 89 44 24 ? 48 8B 4C 24 ? 48 C1 E9 20 8D 3C 09 4A 03 7C C0 ? 0F B7 17 C1 EA 06 41 39 6E 04
	NameByIndex = Utils::FindSiganture(xorstr("48 89 5C 24 18 48 89 6C 24 20 56 57 41 56 48 81 EC 40 08"));

	BoneMatrix = Utils::FindSiganture(xorstr("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45"));
	BoneMatrix = RELATIVE_ADDRESS(BoneMatrix, 5);

	ProjectWorldToScreen = Utils::FindSiganture(xorstr("E8 ? ? ? ? 48 8B 5C 24 ? 41 88 07 48 83 C4 30"));
	ProjectWorldToScreen = RELATIVE_ADDRESS(ProjectWorldToScreen, 5);

	LineOfSightToSAi = Utils::FindSiganture(xorstr("48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 20 45 8A E1"));

	UObjectArray = Utils::FindSiganture(xorstr("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1"));
	UObjectArray = RELATIVE_ADDRESS(UObjectArray, 7);

	auto UWorld = *(uint64_t*)(AUWorld);
	auto GameInstance = *(uint64_t*)(UWorld + 0x190);
	auto LocalPlayers = *(uint64_t*)(GameInstance + 0x38);
	auto ULocalPlayer = *(uint64_t*)(LocalPlayers);
	auto UViewportClient = *(uint64_t*)(ULocalPlayer + 0x70);
	auto vpVt = *(void***)(UViewportClient);

	RobotoFont = (UObject*)FindAllObjects(xorstr("Fonts/Roboto_"), xorstr("Roboto_"));
	K2_DrawTextA = FindObject("K2_DrawText");
	K2_DrawLineA = FindObject("K2_DrawLine");

	DWORD PEOutput;
	VirtualProtect(&vpVt[107], 8, 0x40, &PEOutput);
	oPostRender = reinterpret_cast<decltype(oPostRender)>(vpVt[107]);
	vpVt[107] = &hPostRender;
	VirtualProtect(&vpVt[107], 8, PEOutput, 0);

	//speed gay
	MainGay();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		Main();
	}

	return TRUE;
}