#include "utils.hpp"

//https://stackoverflow.com/questions/26572459/c-get-module-base-address-for-64bit-application
DWORD_PTR GetProcessBaseAddress(DWORD processID, const wchar_t* targetModuleName)
{
	DWORD_PTR   baseAddress = 0;
	HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	HMODULE* moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;
	wchar_t moduleName[25] = L"";

	if (processHandle)
	{
		if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE*)moduleArrayBytes;

					if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
					{
						for (unsigned int i = 0; i < moduleCount; i++) {
							GetModuleBaseName(processHandle, moduleArray[i], moduleName, 24);
							if (wcscmp(moduleName, targetModuleName) == 0) baseAddress = (DWORD_PTR)moduleArray[i];
						}
					}
					LocalFree(moduleArrayBytes);
				}
			}
		}
		CloseHandle(processHandle);
	}

	return baseAddress;
}

DWORD_PTR GetGameAssemblyBase(const wchar_t* targetModuleName) {
	return GetProcessBaseAddress(GetCurrentProcessId(), targetModuleName);
}

const char* returnRoleName(int id) {
	switch (id)
	{
	case gameRoleId::None:
		return("None");
		break;
	case gameRoleId::Goose:
		return("Goose");
		break;
	case gameRoleId::Duck:
		return("Duck");
		break;
	case gameRoleId::Dodo:
		return("Dodo");
		break;
	case gameRoleId::Bounty:
		return("Bounty");
		break;
	case gameRoleId::Mechanic:
		return("Mechanic");
		break;
	case gameRoleId::Technician:
		return("Technician");
		break;
	case gameRoleId::Medium:
		return("Medium");
		break;
	case gameRoleId::Vigilante:
		return("Vigilante");
		break;
	case gameRoleId::Cannibal:
		return("Cannibal");
		break;
	case gameRoleId::Morphling:
		return("Morphling");
		break;
	case gameRoleId::Sheriff:
		return("Sheriff");
		break;
	case gameRoleId::Silencer:
		return("Silencer");
		break;
	case gameRoleId::Canadian:
		return("Canadian");
		break;
	case gameRoleId::LoverDuck:
		return("LoverDuck");
		break;
	case gameRoleId::LoverGoose:
		return("LoverGoose");
		break;
	case gameRoleId::Vulture:
		return("Vulture");
		break;
	case gameRoleId::Professional:
		return("Professional");
		break;
	case gameRoleId::Spy:
		return("Spy");
		break;
	case gameRoleId::Mimic:
		return("Mimic");
		break;
	case gameRoleId::Detective:
		return("Detective");
		break;
	case gameRoleId::Pigeon:
		return("Pigeon");
		break;
	case gameRoleId::Birdwatcher:
		return("Birdwatcher");
		break;
	case gameRoleId::Assassin:
		return("Assassin");
		break;
	case gameRoleId::Falcon:
		return("Falcon");
		break;
	case gameRoleId::Hitman:
		return("Hitman");
		break;
	case gameRoleId::Bodyguard:
		return("Bodyguard");
		break;
	case gameRoleId::Snitch:
		return("Snitch");
		break;
	case gameRoleId::Politician:
		return("Politician");
		break;
	case gameRoleId::Locksmith:
		return("Locksmith");
		break;
	case gameRoleId::Mortician:
		return("Mortician");
		break;
	case gameRoleId::Celebrity:
		return("Celebrity");
		break;
	case gameRoleId::Party:
		return("Party");
		break;
	case gameRoleId::Demolitionist:
		return("Demolitionist");
		break;
	case gameRoleId::DuelingDodo:
		return("DuelingDodo");
		break;
	case gameRoleId::GHGoose:
		return("GHGoose");
		break;
	case gameRoleId::GHDuck:
		return("GHDuck");
		break;
	case gameRoleId::GHBounty:
		return("GHBounty");
		break;
	case gameRoleId::HNSGoose:
		return("HNSGoose");
		break;
	case gameRoleId::HNSDuck:
		return("HNSDuck");
		break;
	case gameRoleId::HNSBounty:
		return("HNSBounty");
		break;
	case gameRoleId::DNDDuck:
		return("DNDDuck");
		break;
	case gameRoleId::DNDFalcon:
		return("DNDFalcon");
		break;
	case gameRoleId::DNDVulture:
		return("DNDVulture");
		break;
	case gameRoleId::DNDMorphling:
		return("DNDMorphling");
		break;
	case gameRoleId::FPGoose:
		return("FPGoose");
		break;
	case gameRoleId::ExploreGoose:
		return("ExploreGoose");
		break;
	case gameRoleId::TTVampire:
		return("TTVampire");
		break;
	case gameRoleId::TTPeasant:
		return("TTPeasant");
		break;
	case gameRoleId::TTThrall:
		return("TTThrall");
		break;
	case gameRoleId::Spectator:
		return("Spectator");
		break;
	case gameRoleId::IdentityThief:
		return("IdentityThief");
		break;
	case gameRoleId::Adventurer:
		return("Adventurer");
		break;
	case gameRoleId::Avenger:
		return("Avenger");
		break;
	case gameRoleId::Ninja:
		return("Ninja");
		break;
	case gameRoleId::Undertaker:
		return("Undertaker");
		break;
	case gameRoleId::Snoop:
		return("Snoop");
		break;
	case gameRoleId::Esper:
		return("Esper");
		break;
	case gameRoleId::Invisibility:
		return("Invisibility");
		break;
	case gameRoleId::Astral:
		return("Astral");
		break;
	case gameRoleId::Pelican:
		return("Pelican");
		break;
	case gameRoleId::TTEThrall:
		return("TTEThrall");
		break;
	case gameRoleId::TTMummy:
		return("TTMummy");
		break;
	case gameRoleId::SerialKiller:
		return("SerialKiller");
		break;
	case gameRoleId::Engineer:
		return("Engineer");
		break;
	case gameRoleId::Warlock:
		return("Warlock");
		break;
	case gameRoleId::StreetUrchin:
		return("StreetUrchin");
		break;
	case gameRoleId::Tracker:
		return("Tracker");
		break;


	default:
		return("None");
		break;
	}
}

bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;


	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}