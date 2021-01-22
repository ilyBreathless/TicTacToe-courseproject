
#include <Windows.h>
#include "stdafx.h"
#include "TicTacToe.h"
#include "resource.h"
#pragma comment(lib,"msimg32.lib")
enum EMarker { keBlank, keX, keO };
#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
BLENDFUNCTION blf = { AC_SRC_OVER,0, 255, 0 };
bool forward = true;
HDC hdc;
int alpha = 0;
//bool isDrawing = true;
//HDC backDC;
//HWND mhWnd;
//RECT rc;
//HBITMAP pattern;
HBITMAP krestik;
HBITMAP kruzhok;
//HBITMAP backBMP;
EMarker gePlayerTurn = keX;
EMarker geaaBoard[3][3] =
{ {keBlank, keBlank, keBlank},
{keBlank, keBlank, keBlank},
{keBlank, keBlank, keBlank} };
const int gkiSqrSz = 200;
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
38
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
//void DrawFrameX(HDC hdc int iX, int iY);
int APIENTRY _tWinMain(HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPTSTR lpCmdLine,
int nCmdShow)
{
UNREFERENCED_PARAMETER(hPrevInstance);
UNREFERENCED_PARAMETER(lpCmdLine);
// TODO: Place code here.
MSG msg;
HACCEL hAccelTable;
// Initialize global strings
LoadString(hInstance, IDS_APP_TITLE, szTitle,
MAX_LOADSTRING);
LoadString(hInstance, IDC_TICTACTOE, szWindowClass,
MAX_LOADSTRING);
MyRegisterClass(hInstance);
// Perform application initialization:
if (!InitInstance(hInstance, nCmdShow))
{
return FALSE;
}
hAccelTable = LoadAccelerators(hInstance,
MAKEINTRESOURCE(IDC_TICTACTOE));
// Main message loop:
while (GetMessage(&msg, NULL, 0, 0))
{
if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
}
39
return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
WNDCLASSEX wcex;
wcex.cbSize = sizeof(WNDCLASSEX);
wcex.style = CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc = WndProc;
wcex.cbClsExtra = 0;
wcex.cbWndExtra = 0;
wcex.hInstance = hInstance;
wcex.hIcon = LoadIcon(hInstance,
MAKEINTRESOURCE(IDI_TICTACTOE));
wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TICTACTOE);
wcex.lpszClassName = szWindowClass;
wcex.hIconSm = LoadIcon(wcex.hInstance,
MAKEINTRESOURCE(IDI_SMALL));
return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
HWND hWnd;
hInst = hInstance; // Store instance handle in our global variable
krestik = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
kruzhok = LoadBitmap(hInst,
MAKEINTRESOURCE(IDB_BITMAP3));
40
// pattern = LoadBitmap(hInst,
MAKEINTRESOURCE(IDB_BITMAP2));
hWnd = CreateWindow(szWindowClass, szTitle,
WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT, 0, 620, 670, NULL, NULL, hInstance,
NULL);
if (!hWnd)
{
return FALSE;
}
//mhWnd = hWnd;
ShowWindow(hWnd,
nCmdShow);
UpdateWindow(hWnd);
SetTimer(hWnd, 1000, 1000 / 60, NULL);
// CreateThread(NULL, NULL,
(LPTHREAD_START_ROUTINE)&DrawFrameX, NULL, NULL, NULL);
return TRUE;
}
void DrawBoard(HDC hdc)
{
HPEN hPenOld;
// Draw the board lines
HPEN hLinePen;
COLORREF qLineColor;
qLineColor = RGB(0, 0, 0);
hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
hPenOld = (HPEN)SelectObject(hdc, hLinePen);
// Draw two vertical lines
for (int iX = gkiSqrSz; iX < 3 * gkiSqrSz; iX += gkiSqrSz) {
MoveToEx(hdc, iX, 0, NULL);
LineTo(hdc, iX, 3 * gkiSqrSz);
41
}
// Draw two horizontal lines
for (int iY = gkiSqrSz; iY < 3 * gkiSqrSz; iY += gkiSqrSz) {
MoveToEx(hdc, 0, iY, NULL);
LineTo(hdc, 3 * gkiSqrSz, iY);
}
SelectObject(hdc, hPenOld);
DeleteObject(hLinePen);
}
void DrawXLeft(HDC hdc, int iX, int iY)
{
HPEN hPenOld;
HPEN hLinePen;
HWND hWnd;
COLORREF qLineColor;
qLineColor = RGB(255, 0, 0);
const int kiPenWidth = 10;
hLinePen = CreatePen(PS_SOLID, kiPenWidth, qLineColor);
hPenOld = (HPEN)SelectObject(hdc, hLinePen);
// Get bounds
const int kiLowX = iX * gkiSqrSz + 2 * kiPenWidth;
const int kiHighX = (iX + 1) * gkiSqrSz - 2 * kiPenWidth;
const int kiLowY = iY * gkiSqrSz + 2 * kiPenWidth;
const int kiHighY = (iY + 1) * gkiSqrSz - 2 * kiPenWidth;
BITMAP krestikObject;
GetObject(krestik, sizeof(BITMAP), &krestikObject);
HDC memDC = CreateCompatibleDC(hdc);
SelectObject(memDC, krestik);
alpha = 0;
if (alpha >= 255) forward = false;
if (alpha <= 0) forward = true;
if (forward)
42
alpha += 40; else
alpha -= 20;
// if (alpha = 255) forward = false;
blf.SourceConstantAlpha = alpha;
AlphaBlend(hdc, kiLowX - 15, kiLowY - 15, krestikObject.bmWidth,
krestikObject.bmHeight, memDC, 0, 0, krestikObject.bmWidth,
krestikObject.bmHeight, blf);
DeleteDC(memDC);
Sleep(100);
}
void DrawOon(HDC hdc, int iX, int iY) {
HPEN hPenOld;
HPEN hLinePen;
HWND hWnd;
COLORREF qLineColor;
qLineColor = RGB(255, 0, 0);
const int kiPenWidth = 10;
hLinePen = CreatePen(PS_SOLID, kiPenWidth, qLineColor);
hPenOld = (HPEN)SelectObject(hdc, hLinePen);
// Get bounds
const int kiLowX = iX * gkiSqrSz + 2 * kiPenWidth;
const int kiHighX = (iX + 1) * gkiSqrSz - 2 * kiPenWidth;
const int kiLowY = iY * gkiSqrSz + 2 * kiPenWidth;
const int kiHighY = (iY + 1) * gkiSqrSz - 2 * kiPenWidth;
BITMAP kruzhokObject;
GetObject(kruzhok, sizeof(BITMAP), &kruzhokObject);
HDC memDC = CreateCompatibleDC(hdc);
SelectObject(memDC, kruzhok);
alpha = 0;
if (alpha >= 255) forward = false;
if (alpha <= 0) forward = true;
if (forward)
43
alpha += 40; else
alpha -= 20;
// if (alpha = 255) forward = false;
blf.SourceConstantAlpha = alpha;
AlphaBlend(hdc, kiLowX - 15, kiLowY - 15, kruzhokObject.bmWidth,
kruzhokObject.bmHeight, memDC, 0, 0, kruzhokObject.bmWidth,
kruzhokObject.bmHeight, blf);
DeleteDC(memDC);
Sleep(100);
}
//void DrawO(HDC hdc, int iX, int iY) {
// HPEN hPenOld;
// HPEN hLinePen;
// COLORREF qLineColor;
// qLineColor = RGB(0, 0, 255);
// const int kiPenWidth = 10;
// hLinePen = CreatePen(PS_SOLID, kiPenWidth, qLineColor);
// hPenOld = (HPEN)SelectObject(hdc, hLinePen);
//
// // Get bounds
// const int kiLowX = iX * gkiSqrSz + 2 * kiPenWidth;
// const int kiHighX = (iX + 1) * gkiSqrSz - 2 * kiPenWidth;
// const int kiLowY = iY * gkiSqrSz + 2 * kiPenWidth;
// const int kiHighY = (iY + 1) * gkiSqrSz - 2 * kiPenWidth;
//
// Arc(hdc, kiLowX, kiLowY, kiHighX, kiHighY, 0, 0, 0, 0);
//
// SelectObject(hdc, hPenOld);
// DeleteObject(hLinePen);
//}
bool PlaceX(LPARAM lParam)
{
int iPosX = LOWORD(lParam);
int iPosY = HIWORD(lParam);
int iSqX = (iPosX / gkiSqrSz);
int iSqY = (iPosY / gkiSqrSz);
44
if ((gePlayerTurn == keX) &&
(iSqX < 3) && (iSqY < 3) &&
(geaaBoard[iSqY][iSqX]) == keBlank)
{
geaaBoard[iSqY][iSqX] = keX;
return true;
}
return false;
}
bool PlaceO(LPARAM lParam)
{
int iPosX = LOWORD(lParam);
int iPosY = HIWORD(lParam);
int iSqX = (iPosX / gkiSqrSz);
int iSqY = (iPosY / gkiSqrSz);
if ((gePlayerTurn == keO) &&
(iSqX < 3) && (iSqY < 3) &&
(geaaBoard[iSqY][iSqX]) == keBlank)
{
geaaBoard[iSqY][iSqX] = keO;
return true;
}
return false;
}
bool HasWon() {
// Check for win through upper left
if (geaaBoard[0][0] != keBlank) {
if (geaaBoard[0][1] == geaaBoard[0][0] &&
geaaBoard[0][2] == geaaBoard[0][0])
{
return true;
}
if (geaaBoard[1][0] == geaaBoard[0][0] &&
geaaBoard[2][0] == geaaBoard[0][0])
{
return true;
}
45
}
// Check for win through center
if (geaaBoard[1][1] != keBlank) {
if (geaaBoard[0][0] == geaaBoard[1][1] &&
geaaBoard[2][2] == geaaBoard[1][1])
{
return true;
}
if (geaaBoard[0][2] == geaaBoard[1][1] &&
geaaBoard[2][0] == geaaBoard[1][1])
{
return true;
}
if (geaaBoard[1][0] == geaaBoard[1][1] &&
geaaBoard[1][2] == geaaBoard[1][1])
{
return true;
}
if (geaaBoard[0][1] == geaaBoard[1][1] &&
geaaBoard[2][1] ==
geaaBoard[1][1])
{
return true;
}
}
// Check for win through lower right
if (geaaBoard[2][2] != keBlank) {
if (geaaBoard[2][0] == geaaBoard[2][2] &&
geaaBoard[2][1] == geaaBoard[2][2])
{
return true;
}
if (geaaBoard[0][2] == geaaBoard[2][2] &&
geaaBoard[1][2] == geaaBoard[2][2])
{
return true;
}
}
46
return false;
}
bool IsBoardFull()
{
for (int iX = 0; iX < 3; ++iX) {
for (int iY = 0; iY < 3; ++iY) {
if (geaaBoard[iY][iX] == keBlank) {
return false;
}
}
}
return true;
}
void ResetGame()
{
for (int iX = 0; iX < 3; ++iX) {
for (int iY = 0; iY < 3; ++iY) {
geaaBoard[iY][iX] = keBlank;
}
}
gePlayerTurn = keX;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM
wParam, LPARAM lParam)
{
int wmId, wmEvent;
PAINTSTRUCT ps;
HDC hdc;
switch (message)
{
case WM_TIMER: {
RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
// InvalidateRect(hWnd, &rc, NULL);
break;
47
}
case WM_LBUTTONDOWN:
{
bool bValidMove = PlaceX(lParam);
if (bValidMove) {
gePlayerTurn = keO;
}
else {
::MessageBox(hWnd, _T("Invalid Move"), _T("Error"),
MB_OK);
}
// Repaint the window after the update
InvalidateRect(hWnd, 0, TRUE);
break;
}
case WM_RBUTTONDOWN:
{
bool bValidMove = PlaceO(lParam);
if (bValidMove) {
gePlayerTurn = keX;
}
else {
::MessageBox(hWnd, _T("Invalid Move"), _T("Error"),
MB_OK);
}
// Repaint the window after the update
InvalidateRect(hWnd, 0, TRUE);
break;
}
case WM_COMMAND:
wmId = LOWORD(wParam);
wmEvent = HIWORD(wParam);
// Parse the menu selections:
switch (wmId)
{
case IDM_ABOUT:
DialogBox(hInst,
MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
break;
case IDM_NEW_GAME:
48
SetTimer(hWnd, 1000, 1000 / 60, NULL);
ResetGame();
// Repaint the window after the update
InvalidateRect(hWnd, 0, TRUE);
break;
case IDM_EXIT:
DestroyWindow(hWnd);
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
break;
case WM_PAINT:
{
hdc = BeginPaint(hWnd, &ps);
DrawBoard(hdc);
// Draw Xs and Os
for (int iX = 0; iX < 3; ++iX) {
for (int iY = 0; iY < 3; ++iY) {
if (geaaBoard[iY][iX] == keX) {
DrawXLeft(hdc, iX, iY);
//DrawFrameX(iX, iY);
//DrawXRight(iX, iY);
}
else if (geaaBoard[iY][iX] == keO) {
DrawOon(hdc, iX, iY);
}
}
}
if (HasWon()) {
KillTimer(hWnd, 1000);
if (gePlayerTurn == keO) {
::MessageBox(hWnd, _T("Player 1 Won"), _T("New
Game?"), MB_OK);
49
}
else {
::MessageBox(hWnd, _T("Player 2 Won"), _T("New
Game?"), MB_OK);
}
SetTimer(hWnd, 1000, 1000 / 60, NULL);
ResetGame();
InvalidateRect(hWnd, 0, TRUE);
}
else {
// If there is no win, check for a draw
if (IsBoardFull()) {
KillTimer(hWnd, 1000);
::MessageBox(hWnd, _T("It's a Draw"), _T("New
Game?"), MB_OK);
SetTimer(hWnd, 1000, 1000 / 60, NULL);
ResetGame();
// Repaint the window after the update
InvalidateRect(hWnd, 0, TRUE);
}
}
EndPaint(hWnd, &ps);
}
break;
case WM_DESTROY:
PostQuitMessage(0);
break;
default:
return
DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM
wParam, LPARAM
50
lParam)
{
UNREFERENCED_PARAMETER(lParam);
switch (message)
{
case WM_INITDIALOG:
return (INT_PTR)TRUE;
case WM_COMMAND:
if (LOWORD(wParam) == IDOK || LOWORD(wParam) ==
IDCANCEL)
{
EndDialog(hDlg, LOWORD(wParam));
return (INT_PTR)TRUE;
}
break;
}
return (INT_PTR)FALSE;
}
