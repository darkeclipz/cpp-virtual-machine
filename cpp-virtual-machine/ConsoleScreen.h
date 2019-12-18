#pragma once
#include <windows.h>
#include <string>
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */

const WORD FOREGROUND_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD FOREGROUND_GRAY = 0x08;

const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 40;
const int SPLIT_X = 80;
const int SPLIT_Y = 21;

class ConsoleScreen
{
private:
	int width;
	int height;
	std::string title;
	HANDLE wHnd;
	HANDLE rHnd;
	SMALL_RECT windowSize;
	COORD bufferSize;
	COORD characterBufferSize;
	COORD characterBufferPosition;
	SMALL_RECT consoleWriteArea;
	CHAR_INFO* consoleBuffer;
public:
	ConsoleScreen(short width, short height, std::string title);
	~ConsoleScreen();
	void DrawString(int x, int y, std::string text, WORD attributes);
	void SetAttributes(int x, int y, int n, WORD attributes);
	void SetString(int x, int y, std::string text);
	void SetChar(int x, int y, char c);
	void SetAttribute(int x, int y, WORD attributes);
	void DrawBuffer();
	void ClearBuffer();
	int GetWidth();
	int GetHeight();
};