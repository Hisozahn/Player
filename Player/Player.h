#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

class Player
{
public:
	Player() {};
	Player(LPCWSTR directoryPath, LPCWSTR audioFilePattern);
	~Player();
};

