#pragma once

namespace GooseGooseDuck {

	namespace SpawnedPlayersManager {
		constexpr int AddSpawnedPlayer = 0x7BC710;
		constexpr int spawnedPlayers = 0x18;
	}

	namespace localPlayer {
		constexpr int update = 0xDADE30;
		constexpr int Player = 0x18; // public PlayerController Player;
	}

	namespace unityEngineCamera {
		constexpr int WorldToScreenPoint = 0x306E830; // public Vector3 WorldToScreenPoint(Vector3 position)
	}

	namespace cinemachine {
		constexpr int damp = 0x1E0A050;   //Cinemachine.Utility.Damper.Damp(Vector3 initial, Vector3 dampTime, float deltaTime)
	}

	namespace GameManager { 	// Managers.GameManagers.GameManager
		constexpr int update = 0x7FB370;
		constexpr int gameState = 0x18;
	}

	namespace PlayerController {
		constexpr int updateRVA = 0xDDD5F0; // AOBScan too.
		constexpr int isSilenced = 0xC1; // Boolean
		constexpr int isInfected = 0xC3; // Boolean
		constexpr int playerRoleId = 0xE8; // pointer, [instance+playerRoleId] + 0x10, int
		constexpr int isPlayerRoleSet = 0xF0; // Boolean
		constexpr int inVent = 0xF1; // Boolean
		constexpr int facingRight = 0x130; // Boolean, can make "Moon walk".
		constexpr int hasBomb = 0x134; // Boolean
		constexpr int isGhost = 0x188; // Boolean
		constexpr int isLocal = 0x1C0; // Boolean
		constexpr int nickname = 0x1D0; // pointer, [instance+nickname] +  0x14, length: [instance+nickname] + 0x10
		constexpr int invisibilityDistance = 0x32C; // int, need this?
		constexpr int isSpectator = 0x37A; // Boolean
		constexpr int isRemoteSpectating = 0x37B; // Boolean
		constexpr int position = 0x2C8; // Value name is randomized. x, y. Float, Float
		/*
		[Notable offsets]
		2B0, Player location, Float Float,   x, y
		2BC, using left bool. camera accel?
		Using UnityEngine-Vector3.Slerp for camera movement?
		*/
	}
}
