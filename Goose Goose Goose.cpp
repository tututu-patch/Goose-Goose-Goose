﻿#include "utils.hpp"
#include "struct.hpp"

#include "MinHook/include/MinHook.h"
#include "kiero/kiero.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include <d3d11.h>
#include <iostream>
#include <list>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace std;

typedef HRESULT(WINAPI* PRESENT)(IDXGISwapChain*, UINT, UINT);
typedef LRESULT(WINAPI* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef void* (_stdcall* tFlip)(void* PlayerController, int someval1, int someval2, void* PhotonData); // void Flip(PlayerController this, int NBPPNBKBMNF, int JMAHGLDEHHB, PhotonMessageInfo BJMKOJJELHA){}
typedef void* (_stdcall* tUpdate)(void* PlayerController);

ExampleAppLog appLog;

void* myPlayerController = nullptr; // Handlers.GameHandlers.PlayerHandlers.PlayerController
tFlip oFlip; // RVA = "0x192A290"          AOBScan?
tUpdate oUpdate;

PRESENT oPre = NULL;
WNDPROC oWndproc = NULL;

ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

bool init = false;
bool canRender = true;
bool canDrawESP = true;

list<DWORD_PTR> PlayerControllerList;
list<DWORD_PTR>::iterator ListIterator;

static playerInfo player[16]; // max player is 16.
DWORD_PTR LocalPlayerController = 0;

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Don't ignore closing window even the menu opened.

	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
		canRender = !canRender;
		return false;
	}

	if (canRender) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndproc, hWnd, uMsg, wParam, lParam);

}

tFlip hkFlip(void* PlayerController, int someval1, int someval2, void* PhotonData) {

	// useless for now on.
	/*int cnt = 0;
	for (ListIterator = PlayerControllerList.begin(); ListIterator != PlayerControllerList.end(); ListIterator++) {
		if ((DWORD_PTR)PlayerController != *ListIterator) cnt++;
	}

	if (PlayerControllerList.size() == 0)
	{
		PlayerControllerList.push_back((DWORD_PTR)PlayerController);
		//appLog.AddLog("[info] Add new PlayerController: %12llX\n", (DWORD_PTR)PlayerController);
	}
	if (PlayerControllerList.size() != 0 && PlayerControllerList.size() == cnt)
	{
		PlayerControllerList.push_back((DWORD_PTR)PlayerController);
		//appLog.AddLog("[info] Add new PlayerController: %12llX\n", (DWORD_PTR)PlayerController);
	}*/

	return (tFlip)oFlip(PlayerController, someval1, someval2, PhotonData);
}

tUpdate hkUpdate(void* PlayerController)
{

	if (*(bool*)((DWORD_PTR)PlayerController + GooseGooseDuck::PlayerController::isLocal))
		LocalPlayerController = (DWORD_PTR)PlayerController;

	static list<DWORD_PTR>::iterator tmpIter;
	tmpIter = std::find(PlayerControllerList.begin(), PlayerControllerList.end(), (DWORD_PTR)(PlayerController));

	if (tmpIter == PlayerControllerList.end()) {
		PlayerControllerList.push_back((DWORD_PTR)PlayerController);
	}

	int cnt = 0;

	for (ListIterator = PlayerControllerList.begin(); ListIterator != PlayerControllerList.end(); ListIterator++) {
		player[cnt].update(*ListIterator); // wrong reference issue
		player[cnt].updatePosition(*ListIterator);
		cnt++;
	}

	return (tUpdate)oUpdate(PlayerController);
}

HRESULT WINAPI hkPre(IDXGISwapChain* pSC, UINT SyncInterval, UINT Flags)
{

	//https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook/blob/master/ImGui%20DirectX%2011%20Kiero%20Hook/main.cpp
	if (!init)
	{
		if (SUCCEEDED(pSC->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSC->GetDesc(&sd);
			ID3D11Texture2D* pBackBuffer;
			pSC->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();

			oWndproc = (WNDPROC)SetWindowLongPtr(FindWindow(0, L"Goose Goose Duck"), GWLP_WNDPROC, (LONG_PTR)WndProc);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesThai());
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
			ImGui_ImplWin32_Init(FindWindow(0, L"Goose Goose Duck"));
			ImGui_ImplDX11_Init(pDevice, pContext);

			ImGui::StyleColorsDark();

			init = true;
		}
		else
			return oPre(pSC, SyncInterval, Flags);
	}

	if (canRender || canDrawESP) {

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (canRender)
		{

			static int CurrentIdx = -1;

			if (PlayerControllerList.size() > 16)
			{
				PlayerControllerList.clear();
			}

			{
				ImGui::Begin("Log window");
				ImGui::End();
				appLog.Draw("Log window");
			}

			{
				static int cnt = 0;
				ImGui::Begin("Player list");

				if (ImGui::Button("Clear")) {
					PlayerControllerList.clear();
					for (int i = 0; i < PlayerControllerList.size(); i++) { player[i].reset(); }
					CurrentIdx = 0;
				}

				ImGui::SameLine();
				if (ImGui::Button("All roles")) {
					for (int i = 0; i < PlayerControllerList.size(); i++) {
						appLog.AddLog("[Player Info] Name: %s\t\tRole: %s\n", player[i].nickname, player[i].roleName);
					}
				}

				if (ImGui::BeginListBox("PlayerControllerDataListBox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
					cnt = 0;

					for (ListIterator = PlayerControllerList.begin(); ListIterator != PlayerControllerList.end(); ListIterator++) {
						const bool is_selected = (CurrentIdx == cnt);
						player[cnt].update(*ListIterator); // wrong reference == cause crash

						if (ImGui::Selectable(player[cnt].nickname, is_selected)) {
							ImGui::SetItemDefaultFocus();
							CurrentIdx = cnt;
							appLog.AddLog(u8"\n[Player Info]\n"
								"PlayerController: %012llX\n"
								"Nickname: %s\n"
								"isRoleSet: %s\n"
								"RoleName: %s\n"
								"inVent: %s\n"
								"isGhost: %s\n"
								"isInfected :%s\n"
								"isLocal: %s\n"
								"isSilenced: %s\n"
								"isSpectator: %s\n"
								"position X: %f, Y: %f\n",
								player[cnt].ptrPlayerController,
								player[cnt].nickname,
								player[cnt].isPlayerRoleSet ? "True" : "False",
								player[cnt].roleName,
								player[cnt].inVent ? "True" : "False",
								player[cnt].isGhost ? "True" : "False",
								player[cnt].isInfected ? "True" : "False",
								player[cnt].isLocal ? "True" : "False",
								player[cnt].isSilenced ? "True" : "False",
								player[cnt].isSpectator ? "True" : "False",
								player[cnt].pos.x,
								player[cnt].pos.y);
						}
						cnt++;
					}
				}
				ImGui::End();
			}

		}

		if (canDrawESP) {
			{
				ImGui::SetNextWindowPos(ImVec2(0, 0));
				ImGui::SetNextWindowSize(ImVec2(1280, 720)); // 640, 360
				ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				const ImU32 col = ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));   //RGBA
				int cnt = 0;
				positionXY LocalPlayerPos = { 0.0f, 0.0f };


				for (ListIterator = PlayerControllerList.begin(); ListIterator != PlayerControllerList.end(); ListIterator++) { // local-based-relative calculation needed.

					if (player[cnt].isLocal) {
						//draw_list->AddRect(ImVec2(600, 310), ImVec2(675, 450), col, 0.0f, ImDrawFlags_None, 3.0f); // All visible things are resized in reverse proportion to local player speed
						draw_list->AddLine(ImVec2(640- 5, 360), ImVec2(640 + 5, 360 + 10), ImColor(1.0f, 0.0f, 0.0f, 1.0f), 2.0f);
					}
					else {
						float deltaX, deltaY;
						memcpy(&LocalPlayerPos, (int*)(LocalPlayerController + GooseGooseDuck::PlayerController::position), 8);
						
						deltaX = player[cnt].pos.x - LocalPlayerPos.x;
						deltaY = player[cnt].pos.y - LocalPlayerPos.y;

						draw_list->AddLine(ImVec2(640, 360), ImVec2(640+deltaX*80, 360+deltaY*-80), ImColor(0.4f, 1.0f, 0.4f, 1.0f), 2.0f);
					}
					cnt++;
				}
			}
		}

		ImGui::Render();
		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return oPre(pSC, SyncInterval, Flags);
}

void MainFunc(HMODULE hModule) {

	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{

		bool hooked = true;

		// define KIERO_USE_MINHOOK must be 1
		// the index of the required function can be found in the METHODSTABLE.txt
		kiero::bind(8, (void**)&oPre, hkPre);

		if (MH_CreateHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + GooseGooseDuck::PlayerController::flipRVA), hkFlip, (void**)&oFlip) != MH_OK
			|| MH_EnableHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + GooseGooseDuck::PlayerController::flipRVA)) != MH_OK) {
			appLog.AddLog("[Error] Can't create or enable Flip hook.\n");
			hooked = false;
		}
		else {
			appLog.AddLog("[Info] Successfully create and enable Flip hook.\n");
		}

		if (MH_CreateHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + GooseGooseDuck::PlayerController::updateRVA), hkUpdate, (void**)&oUpdate) != MH_OK
			|| MH_EnableHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + GooseGooseDuck::PlayerController::updateRVA)) != MH_OK) {
			appLog.AddLog("[Error] Can't create or enable Update hook.\n");
			hooked = false;
		}
		else {
			appLog.AddLog("[Info] Successfully create and enable Update hook.\n");
		}

		if (hooked) {
			appLog.AddLog("\n\nGoose Goose Goose, an open source program made by roy6307.\nYou can review codes on https://github.com/roy6307/Goose-Goose-Goose\n\n");
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		setlocale(LC_ALL, ""); // really need?
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainFunc, hModule, 0, NULL);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
