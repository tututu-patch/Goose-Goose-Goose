#include "playerController.hpp"


typedef void* (_stdcall* tUpdate)(void* PlayerController);

tUpdate oUpdate;


playerInfo player[16]; // max player is 16.

list<DWORD_PTR> PlayerControllerList;
list<DWORD_PTR>::iterator ListIterator;


tUpdate hkUpdate(void* PlayerController)
{

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

bool playerControllerHook() {
	if (MH_CreateHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + GooseGooseDuck::PlayerController::updateRVA), hkUpdate, (void**)&oUpdate) != MH_OK
		|| MH_EnableHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + GooseGooseDuck::PlayerController::updateRVA)) != MH_OK) {
		return false;
	}
	else
		return true;
}