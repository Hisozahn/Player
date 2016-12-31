#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <vector>

#define MAX_COMMAND_SIZE 1000

using namespace std;

typedef struct Track {
	LONG handler;
	TCHAR path[MAX_PATH];
} Track;

class Player
{

private:
	vector<Track> m_tracks = vector<Track>();
	vector<Track> m_activeTracks = vector<Track>();

public:

	Player() {};
	Track load(LPCWSTR filePath);
	vector<Track> load(LPCWSTR directoryPath, LPCWSTR audioFilePattern);

	void unload(Track track);

	void play(Track track);
	void playRandom();
	void stop();

	~Player();
};

