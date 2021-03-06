#include "Main.h"

#define BLUE 252
#define WHITE 255

const TCHAR windowClass[20] = _T("PLAYER");
const TCHAR windowName[20] = _T("Player 1");

const TCHAR backgroundBitmap[30] = _T("Images\\back.bmp");
const TCHAR activeButtonBitmap[30] = _T("Images\\active.bmp");
const TCHAR disabledButtonBitmap[30] = _T("Images\\disabled.bmp");

const TCHAR windowMaskBitmap[30] = _T("Images\\windowMask.bmp");
const TCHAR buttonMaskBitmap[30] = _T("Images\\buttonMask.bmp");

int  APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nShow) {
	srand(time(NULL));

	HBITMAP windowMask = (HBITMAP)LoadImage(NULL, windowMaskBitmap, IMAGE_BITMAP, 0, 0
		, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP buttonMask = (HBITMAP)LoadImage(NULL, buttonMaskBitmap, IMAGE_BITMAP, 0, 0
		, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP back = (HBITMAP)LoadImage(NULL, backgroundBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP active = (HBITMAP)LoadImage(NULL, activeButtonBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP disabled = (HBITMAP)LoadImage(NULL, disabledButtonBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	PlayerWindow window = PlayerWindow(windowMask, WHITE, back);

	static Player player = Player();
	static vector<Track> tracks = player.load(_T("Sounds"), _T("*.wav"));

	ShapedButton button1 = ShapedButton(buttonMask, BLUE, active, disabled);
	button1.setOffset(215, 206);
	
	button1.setClickHandler([](void) -> void {
		player.stop();
		player.playRandom();
	});

	window.addButton(button1);
	
	window.CreateEx(0, windowClass, windowName, WS_POPUP, 0, 0, 800, 800, NULL, NULL, hInstance);

	window.ShowWindow(nShow);
	window.UpdateWindow();
	
	return window.MessageLoop();
}

/*#define MAX_TITLE_SIZE 100
#define BITMAP_FILE_BACKGROUND L"back.bmp"
#define BITMAP_FILE_ACTIVE_BUTTON L"active.bmp"
#define BITMAP_FILE_DISABLED_BUTTON L"disabled.bmp"
#define BITMAP_FILE_MASK L"mask.bmp"
#define BITMAP_FILE_BUTTON_MASK L"buttonMask.bmp"
#define WINDOW_NAME L"Window"
#define BLUE 252
#define WHITE 255
#define MAX_SOUNDS 50
#define MAX_SOUNDS_DIGITS_COUNT 2
#define MAX_COMMAND_LENGTH 400
#define NO_PLAYING_SOUND -1
//#define DEFAULT_WINDOW_HEIGHT 400
//#define DEFAULT_WINDOW_WIDTH 400

enum cursorState {
ARROW,
HAND
};


HINSTANCE g_hInstance;
const TCHAR g_szWindowClass[MAX_TITLE_SIZE] = L"STAR";
LONG g_windowHeight = 400;
LONG g_windowWidth = 400;
HBITMAP g_hBmpAllMask = NULL;
HBITMAP g_hBmpButtonMask = NULL;
HBITMAP g_hBmpBackground = NULL;
HBITMAP g_hBmpActiveButton = NULL;
HWND g_hWnd = NULL;
BYTE* g_pButtonBits = NULL;
int bitMaskWidth = 0;
cursorState g_cursorState = ARROW;
int g_soundsCount = 0;
int g_activeSound = -1;
TCHAR g_soundPathes[MAX_SOUNDS][MAX_PATH];

void handleError(LPCWSTR message) {
MessageBox(g_hWnd, message, L"Error", NULL);
}

void InitSounds() {
HANDLE hFind = INVALID_HANDLE_VALUE;
WIN32_FIND_DATA ffd;
TCHAR szDir[MAX_PATH] = L"Sounds\\*.wav";

hFind = FindFirstFile(szDir, &ffd);

if (hFind == INVALID_HANDLE_VALUE)
{
handleError(TEXT("FindFirstFile"));
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
if ( (errorCode = mciSendString(command, NULL, 0, NULL)) != 0) {
mciGetErrorString(errorCode, errorBuffer, MAX_COMMAND_LENGTH);
handleError(errorBuffer);
}
}



FindClose(hFind);
}

BOOL isCursorOnButton(int x, int y) {
if (g_pButtonBits == NULL || bitMaskWidth == 0
|| y >= g_windowHeight || x >= g_windowWidth) {
return FALSE;
}
else {
return (g_pButtonBits[y * bitMaskWidth + x / 8] >> (7 - (x % 8))) % 2 == 0;
}
}

void OnDraw(HDC hdc) {
if (g_hBmpBackground == NULL) {
return;
}

BITMAP bitmap;
GetObject(g_hBmpBackground, sizeof(bitmap), &bitmap);

HDC hdcMem = CreateCompatibleDC(hdc);
HGDIOBJ oldBitmap = SelectObject(hdcMem, g_hBmpBackground);

HDC hdcBuffer = CreateCompatibleDC(hdc);
HBITMAP bufferBitmap = CreateCompatibleBitmap(hdc, bitmap.bmWidth, bitmap.bmHeight);
SelectObject(hdcBuffer, bufferBitmap);

MaskBlt(hdcBuffer, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, g_hBmpAllMask, 0, 0, SRCCOPY);
if (g_activeSound != NO_PLAYING_SOUND) {
SelectObject(hdcMem, g_hBmpActiveButton);
BitBlt(hdcBuffer, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCINVERT);
SelectObject(hdcMem, g_hBmpButtonMask);
BitBlt(hdcBuffer, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCAND);
SelectObject(hdcMem, g_hBmpActiveButton);
BitBlt(hdcBuffer, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCINVERT);
}
BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcBuffer, 0, 0, SRCCOPY);

SelectObject(hdcMem, oldBitmap);
DeleteDC(hdcBuffer);
DeleteObject(bufferBitmap);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
PAINTSTRUCT ps;
HDC hdc;
int x, y, rnd, errorCode;
BOOL cursorOnButton;
TCHAR command[MAX_COMMAND_LENGTH];
TCHAR buf[MAX_SOUNDS_DIGITS_COUNT + 1];
TCHAR errorBuffer[MAX_COMMAND_LENGTH];
switch (message)
{
case WM_SETCURSOR:
break;
case WM_MOUSEMOVE:
x = LOWORD(lParam);
y = HIWORD(lParam);
cursorOnButton = isCursorOnButton(x, y);
if (cursorOnButton && g_cursorState == ARROW) {
SetCursor(LoadCursor(NULL, IDC_HAND));
g_cursorState = HAND;
}
else if (!cursorOnButton && g_cursorState == HAND) {
SetCursor(LoadCursor(NULL, IDC_ARROW));
g_cursorState = ARROW;
}
break;
case MM_MCINOTIFY:
if (wParam == MCI_NOTIFY_SUCCESSFUL) {
wcscpy_s(command, L"seek ");
_stprintf_s(buf, _T("%d"), g_activeSound);
wcscat_s(command, buf);
wcscat_s(command, L" to start");
if ((errorCode = mciSendString(command, NULL, 0, NULL)) != 0) {
mciGetErrorString(errorCode, errorBuffer, MAX_COMMAND_LENGTH);
handleError(errorBuffer);
}
g_activeSound = NO_PLAYING_SOUND;
InvalidateRect(hWnd, NULL, FALSE);
//OutputDebugString(_T("Avoken\n"));
}
else {
OutputDebugString(_T("Unsuccessful\n"));
}
break;
case WM_LBUTTONDOWN:
x = LOWORD(lParam);
y = HIWORD(lParam);
if (isCursorOnButton(x, y)) {
if (g_activeSound != NO_PLAYING_SOUND) {
wcscpy_s(command, L"stop ");
_stprintf_s(buf, _T("%d"), g_activeSound);
wcscat_s(command, buf);
mciSendString(command, NULL, 0, NULL);
wcscpy_s(command, L"seek ");
_stprintf_s(buf, _T("%d"), g_activeSound);
wcscat_s(command, buf);
wcscat_s(command, L" to start");
if ((errorCode = mciSendString(command, NULL, 0, NULL)) != 0) {
mciGetErrorString(errorCode, errorBuffer, MAX_COMMAND_LENGTH);
handleError(errorBuffer);
}
g_activeSound = NO_PLAYING_SOUND;
}
else {
rnd = rand() % (g_soundsCount);
wcscpy_s(command, L"play ");
_stprintf_s(buf, _T("%d"), rnd);
wcscat_s(command, buf);
wcscat_s(command, L" notify");
mciSendString(command, NULL, 0, hWnd);
//OutputDebugString(buf);
g_activeSound = rnd;
}
//g_playState = g_playState == PLAY ? STOP : PLAY;
InvalidateRect(hWnd, NULL, FALSE);
}
break;

case WM_PAINT:
hdc = BeginPaint(hWnd, &ps);
OnDraw(hdc);
EndPaint(hWnd, &ps);
break;

case WM_CREATE:
g_hBmpBackground = (HBITMAP)LoadImage(NULL, BITMAP_FILE_BACKGROUND, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
if (g_hBmpBackground == NULL) {
handleError(L"Background image not loaded");
}
g_hBmpActiveButton = (HBITMAP)LoadImage(NULL, BITMAP_FILE_ACTIVE_BUTTON, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
if (g_hBmpActiveButton == NULL) {
handleError(L"Image with active button not loaded");
}
srand(time(NULL));
break;

case WM_DESTROY:
DeleteObject(g_hBmpAllMask);
DeleteObject(g_hBmpBackground);
PostQuitMessage(0);
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}


int GetWndClassEx(WNDCLASSEX & wcex) {
memset(&wcex, 0, sizeof(WNDCLASSEX));

wcex.cbSize = sizeof(WNDCLASSEX);
wcex.style = CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc = WndProc;
wcex.cbClsExtra = 0;
wcex.cbWndExtra = 0;
wcex.hInstance = g_hInstance;
wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wcex.lpszMenuName = NULL;
wcex.lpszClassName = g_szWindowClass;
wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

if (!RegisterClassEx(&wcex))
return 0;
return 1;
}

HRGN ExtendRgn(HRGN hRgn, int x1, int y1, int x2, int y2) {
HRGN newHRgn = hRgn;
if (newHRgn == 0) {
newHRgn = CreateRectRgn(x1, y1, x2, y2);
}
else {
CombineRgn(newHRgn, newHRgn, CreateRectRgn(x1, y1, x2, y2), RGN_OR);
}
return newHRgn;
}

void setBitMaskBit(BYTE* pAllBits, LONG widthInBytes, LONG heightIdx, LONG widthIdxInBits) {
LONG ByteWithNeededBit = heightIdx * widthInBytes + widthIdxInBits / 8;
pAllBits[ByteWithNeededBit] += 1 << (7 - (widthIdxInBits % 8));
}

HRGN CreateRegionByMask(LPCWSTR path) {
BITMAP bm;
HBITMAP hMask = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0
, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

GetObject(hMask, sizeof(bm), &bm);

bitMaskWidth = (bm.bmWidth % 16 == 0 ? bm.bmWidth / 16 : bm.bmWidth / 16 + 1) * 2;
BYTE* pAllBits = new unsigned char[bm.bmHeight * bitMaskWidth];
g_pButtonBits = new unsigned char[bm.bmHeight * bitMaskWidth];
memset(pAllBits, 0, sizeof(unsigned char) * bm.bmHeight * bitMaskWidth);
memset(g_pButtonBits, 0, sizeof(unsigned char) * bm.bmHeight * bitMaskWidth);

g_windowHeight = bm.bmHeight;
g_windowWidth = bm.bmWidth;

BYTE* pMaskBits = (BYTE*)bm.bmBits;

HRGN hRgn = 0;
int pixel;
int xStart = -1;

for (LONG heightIdx = 0; heightIdx < bm.bmHeight; heightIdx++) {
for (LONG widthIdx = 0; widthIdx < bm.bmWidth; widthIdx++) {
pixel = pMaskBits[heightIdx * bm.bmWidth + widthIdx];
if (pixel != WHITE) {
setBitMaskBit(pAllBits, bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
}
if (pixel != BLUE) {
setBitMaskBit(g_pButtonBits, bitMaskWidth, bm.bmHeight - heightIdx - 1, widthIdx);
}

if (pixel != WHITE && xStart == -1) {
xStart = widthIdx;
}
else if (pixel == WHITE && xStart != -1) {
hRgn = ExtendRgn(hRgn, xStart, bm.bmHeight - heightIdx - 1
, widthIdx, bm.bmHeight - heightIdx);
xStart = -1;
}
}
if (xStart != -1) {
hRgn = ExtendRgn(hRgn, xStart, bm.bmHeight - heightIdx - 1
, bm.bmWidth, bm.bmHeight - heightIdx);
xStart = -1;
}
}

g_hBmpAllMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, pAllBits);
g_hBmpButtonMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, g_pButtonBits);

DeleteObject(hMask);

return hRgn;
}*/