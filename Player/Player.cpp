#include "Player.h"

Track Player::load(LPCWSTR filePath) {
	Track newTrack;
	memset(&newTrack, 0, sizeof(Track));
	LONG newHandler = m_tracks.size();

	TCHAR mciCommand[MAX_COMMAND_SIZE];
	_stprintf_s(
		mciCommand,
		_T("open %s alias %ld"),
		filePath,
		newHandler
	);
	if (mciSendString(mciCommand, NULL, 0, NULL) == 0) {
		_tcscpy_s(newTrack.path, filePath);
		newTrack.handler = newHandler;

		m_tracks.push_back(newTrack);
		return newTrack;
	}
	else {
		return newTrack;
	}
}

vector<Track> Player::load(LPCWSTR directoryPath, LPCWSTR audioFilePattern) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];

	wcscpy_s(szDir, directoryPath);
	wcscat_s(szDir, _T("\\"));
	wcscat_s(szDir, audioFilePattern);

	hFind = FindFirstFile(szDir, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return {};
	}

	Track trackIterator;
	vector<Track> tracksToReturn = vector<Track>();
	TCHAR filePath[MAX_PATH];
	do {
		_stprintf_s(
			filePath,
			_T("%s\\%s"),
			directoryPath,
			ffd.cFileName
		);
		if ((trackIterator = load(filePath)).path != NULL) {
			tracksToReturn.push_back(trackIterator);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	return tracksToReturn;
}

void Player::unload(Track track) {
	//TODO
}

void Player::play(Track track) {
	if (m_tracks.size() == 0) {
		return;
	}
	TCHAR mciCommand[MAX_COMMAND_SIZE];
	_stprintf_s(
		mciCommand,
		_T("seek %d to start"),
		track.handler
	);
	mciSendString(mciCommand, NULL, 0, NULL);
	_stprintf_s(
		mciCommand,
		_T("play %d"),
		track.handler
	);
	if (mciSendString(mciCommand, NULL, 0, NULL) == 0) {
		m_activeTracks.push_back(track);
	}
}

void Player::playRandom() {
	if (m_tracks.size() == 0) {
		return;
	}
	LONG random = rand() % m_tracks.size();
	play(m_tracks.at(random));
}

void Player::stop() {
	TCHAR mciCommand[MAX_COMMAND_SIZE];
	vector<Track>::iterator trackIterator;
	for (trackIterator = m_activeTracks.begin(); trackIterator != m_activeTracks.end(); ++trackIterator) {
		_stprintf_s(
			mciCommand,
			_T("stop %d"),
			trackIterator->handler
		);
		mciSendString(mciCommand, NULL, 0, NULL);
	}
}

Player::~Player() {
	m_tracks.clear();
}
