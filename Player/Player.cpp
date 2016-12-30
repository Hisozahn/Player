#include "Player.h"

/*Player::Player(LPCWSTR directoryPath, LPCWSTR audioFilePattern) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	
	wcscpy_s(szDir, directoryPath);
	wcscat_s(szDir, _T("\\"));
	wcscat_s(szDir, audioFilePattern);

	hFind = FindFirstFile(szDir, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}
	int i = 0;
	do {
		wcscpy_s(g_soundPathes[i], ffd.cFileName);
		i++;
	} while (i < MAX_SOUNDS && FindNextFile(hFind, &ffd) != 0);

	g_soundsCount = i;

	TCHAR command[MAX_COMMAND_LENGTH];
	TCHAR buf[MAX_SOUNDS_DIGITS_COUNT + 1];
	TCHAR errorBuffer[MAX_COMMAND_LENGTH];
	int errorCode = 0;
	for (i = 0; i < g_soundsCount; i++) {
		wcscpy_s(command, L"open Sounds\\");
		wcscat_s(command, g_soundPathes[i]);
		wcscat_s(command, L" alias ");
		_stprintf_s(buf, _T("%d"), i);
		wcscat_s(command, buf);
		if ((errorCode = mciSendString(command, NULL, 0, NULL)) != 0) {
			mciGetErrorString(errorCode, errorBuffer, MAX_COMMAND_LENGTH);
			handleError(errorBuffer);
		}
	}



	FindClose(hFind);
}*/


Player::~Player()
{
}
