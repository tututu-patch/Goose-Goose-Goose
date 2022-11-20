#include "localPlayer.hpp"

typedef void* (_stdcall* tLPUpdate)(void* instance);

tLPUpdate oLPUpdate;

DWORD_PTR ins;

tLPUpdate hkLPUpdate(void* instance) {
	ins = (DWORD_PTR)instance;
	return (tLPUpdate)oLPUpdate(instance);
}

DWORD_PTR retIns() {
	return ins;
}

bool localPlayerHook() {
	if(MH_CreateHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + 0xBE48D0), hkLPUpdate, (void**)&oLPUpdate) != MH_OK
		|| MH_EnableHook((void*)(GetGameAssemblyBase(L"GameAssembly.dll") + 0xBE48D0)) != MH_OK) {
	return false;
}
	else
	return true;
}