#include <windows.h>
#include <wincon.h>
#include "GameView.h"

wstring s2ws(const string& str)
{
	int len;
	int slength = (int)str.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}

void ConsoleView::Draw(rotation rot, int x, int y, int currentShip, char **field, map<int, int> fleet, string name)
{
	system("CLS");
	cout << name << endl << "SELECT PLACE FOR " + to_string(currentShip) + "-SIZE SHIP" << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (rot == hor)
			{
				if (j - x < currentShip && j - x >= 0 && y == i)
				{
					cout << 'X';
				}
				else
				{
					cout << field[i][j];
				}
			}
			else // rot == vert
			{
				if (i - y < currentShip && i - y >= 0 && x == j)
				{
					cout << 'X';
				}
				else
				{
					cout << field[i][j];
				}
			}
		}
		if (i < 4)
		{
			cout << "	 " + to_string(i + 1) + "-SIZE SHIPS LEFT: " + to_string(fleet[i + 1]);
		}
		cout << endl;
	}
}

void ConsoleView::Draw(char** field)
{
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << field[i][j];
		}
		cout << endl;
	}
}

void ConsoleView::Draw(char** field, char **attackField, string name)
{
	system("CLS");
	cout << name + " FIELD:" << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << field[i][j];
		}
		cout << endl;
	}
	cout << "ENEMY FIELD" << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << attackField[i][j];
		}
		cout << endl;
	}
}

void ConsoleView::Draw(int x, int y, char** field, char** attackField, string name)
{
	system("CLS");
	cout << name << endl << "YOUR FIELD" << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << field[i][j];
		}
		cout << endl;
	}
	cout << "ENEMY FIELD" << endl << "CHOOSE CELL TO ATTACK" << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (x == j && y == i)
			{
				cout << 'X';
			}
			else
			{
				cout << attackField[i][j];
			}
		}
		cout << endl;
	}
}

void ConsoleView::Message(string text1, string text2)
{
	cout << text1 << endl;
	cout << text2 << endl;
}

void ConsoleView::Message(string text)
{
	cout << text << endl;
}

void ConsoleView::ChangePlayer()
{
	system("CLS");
	cout << "GIVE CONTROLLS TO ANOTHER PLAYER AND PRESS ENTER" << endl;
}


GoodView::GoodView()
{
	HWND hwnd = GetConsoleWindow();
	hdc = GetDC(hwnd);

	whitePen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	vertBrush = CreateHatchBrush(HS_VERTICAL, RGB(200, 200, 200));
	horBrush = CreateHatchBrush(HS_HORIZONTAL, RGB(200, 200, 200));
	crossBrush = CreateHatchBrush(HS_CROSS, RGB(200, 200, 200));

	blueBrush = CreateSolidBrush(RGB(0, 100, 255));
	darkBlueBrush = CreateSolidBrush(RGB(10, 0, 60));
	whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	redBrush = CreateSolidBrush(RGB(255, 0, 0));
	backBrush = CreateSolidBrush(RGB(1, 36, 86));

	LOGFONT font;
	font.lfHeight = 40;
	font.lfEscapement = 0;
	font.lfWeight = 1000;
	font.lfItalic = 0;
	font.lfStrikeOut = 0;
	font.lfUnderline = 0;
	font.lfPitchAndFamily = 0;
	hfont = CreateFontIndirect(&font);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 225, 255));
}

void GoodView::Draw(rotation rot, int x, int y, int currentShip, char** field, map<int, int> fleet, string name) //prepare stage
{
	WCHAR str[21];

	SelectObject(hdc, backBrush);
	Rectangle(hdc, 700, 50, 1050, 250);
	Message(name + " FIELD:", "FLEET:");
	Message("SELECT PLACE FOR " + to_string(currentShip) + "-SIZE SHIP");

	for (int i = 1; i <= 4; ++i)
	{
		wsprintf(str, L"%d-SIZE SHIPS LEFT: %d", i, fleet[i]);
		TextOut(hdc, 700, 50 * i, str, 21);
	}

	for (int i = 0; i <= 9; ++i)
	{
		for (int j = 0; j <= 9; ++j)
		{
			if (field[i][j] == 'O')
			{
				SelectObject(hdc, blueBrush);
				Rectangle(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100);
			}
			else if (field[i][j] == '#')
			{
	 			SelectObject(hdc, darkBlueBrush);
				RoundRect(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100, 25, 25);
			}
		}
	}

	SelectObject(hdc, whiteBrush);
	SelectObject(hdc, whitePen);
	if (rot == hor)
	{
		SelectObject(hdc, horBrush);
		for (int i = 0; i < currentShip && x + i <= 9; ++i)
		{
			RoundRect(hdc, 50 + (x + i) * 50, 50 + y * 50, 100 + (x + i) * 50, 100 + y * 50, 25, 25);
		}
	}
	else
	{
		SelectObject(hdc, vertBrush);
		for (int i = 0; i < currentShip && y + i <= 9; ++i)
		{
			RoundRect(hdc, 50 + x * 50, 50 + (y + i) * 50, 100 + x * 50, 100 + (y + i) * 50, 25, 25);
		}
	}
	SelectObject(hdc, blackPen);
}

void GoodView::Draw(char** field)
{
	SelectObject(hdc, blueBrush);
	for (int i = 0; i <= 9; ++i)
	{
		for (int j = 0; j <= 9; ++j)
		{
			Rectangle(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100);
			if (field[i][j] == '#')
			{
				SelectObject(hdc, darkBlueBrush);
				RoundRect(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100, 25, 25);
			}
			else if (field[i][j] == '0')
			{
				SelectObject(hdc, whiteBrush);
				Ellipse(hdc, 60 + j * 50, 60 + i * 50, j * 50 + 90, i * 50 + 90);
			}
			else if (field[i][j] == '@')
			{
				SelectObject(hdc, redBrush);
				RoundRect(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100, 25, 25);
			}
			SelectObject(hdc, blueBrush);
		}
	}
}

void GoodView::Draw(char** field, char** attackField, string name)
{
	Message(name + " FIELD:", "ENEMY FIELD:");
	Draw(field);
	for (int i = 0; i <= 9; ++i)
	{
		for (int j = 0; j <= 9; ++j)
		{
			Rectangle(hdc, j * 50 + 700, i * 50 + 50, j * 50 + 750, i * 50 + 100);
			if (attackField[i][j] == '#')
			{
				SelectObject(hdc, darkBlueBrush);
				RoundRect(hdc, j * 50 + 700, i * 50 + 50, j * 50 + 750, i * 50 + 100, 25, 25);
			}
			else if (attackField[i][j] == '0')
			{
				SelectObject(hdc, whiteBrush);
				Ellipse(hdc, j * 50 + 710, i * 50 + 60, j * 50 + 740, i * 50 + 90);
			}
			else if (attackField[i][j] == '@')
			{
				SelectObject(hdc, redBrush);
				RoundRect(hdc, j * 50 + 700, i * 50 + 50, j * 50 + 750, i * 50 + 100, 25, 25);
			}
			SelectObject(hdc, blueBrush);
		}
	}
}

void GoodView::Draw(int x, int y, char** field, char** attackField, string name)
{
	Message(name + " FIELD:", "ENEMY FIELD:");

	SelectObject(hdc, backBrush);
	Rectangle(hdc, 50, 600, 1150, 650);
	
	SelectObject(hdc, blueBrush);
	for (int i = 0; i <= 9; ++i)
	{
		for (int j = 0; j <= 9; ++j)
		{
			Rectangle(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100);
			if (field[i][j] == '#')
			{
				SelectObject(hdc, darkBlueBrush);
				RoundRect(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100, 25, 25);
			}
			else if (field[i][j] == '0')
			{
				SelectObject(hdc, whiteBrush);
				Ellipse(hdc, 60 + j * 50, 60 + i * 50, j * 50 + 90, i * 50 + 90);
			}
			else if (field[i][j] == '@')
			{
				SelectObject(hdc, redBrush);
				RoundRect(hdc, 50 + j * 50, 50 + i * 50, j * 50 + 100, i * 50 + 100, 25, 25);
			}
			SelectObject(hdc, blueBrush);
		}
	}

	for (int i = 0; i <= 9; ++i)
	{
		for (int j = 0; j <= 9; ++j)
		{
			Rectangle(hdc, j * 50 + 700, i * 50 + 50, j * 50 + 750, i * 50 + 100);
			if (attackField[i][j] == '#')
			{
				SelectObject(hdc, darkBlueBrush);
				RoundRect(hdc, j * 50 + 700, i * 50 + 50, j * 50 + 750, i * 50 + 100, 25, 25);
			}
			else if (attackField[i][j] == '0')
			{
				SelectObject(hdc, whiteBrush);
				Ellipse(hdc, j * 50 + 710, i * 50 + 60, j * 50 + 740, i * 50 + 90);
			}
			else if (attackField[i][j] == '@')
			{
				SelectObject(hdc, redBrush);
				RoundRect(hdc, j * 50 + 700, i * 50 + 50, j * 50 + 750, i * 50 + 100, 25, 25);
			}
			SelectObject(hdc, blueBrush);
		}
	}
	SelectObject(hdc, crossBrush);
	SelectObject(hdc, whitePen);
	Ellipse(hdc, x * 50 + 700, y * 50 + 50, x * 50 + 750, y * 50 + 100);
	SelectObject(hdc, blackPen);
}

void GoodView::Message(string text1, string text2)
{
	SelectObject(hdc, backBrush);
	Rectangle(hdc, 50, 0, 550, 50);
	Rectangle(hdc, 700, 0, 1200, 50);

	wstring wstr = s2ws(text1);
	SelectObject(hdc, hfont);
	TextOut(hdc, 50, 0, wstr.c_str(), wstr.length() + 1);
	wstr = s2ws(text2);
	TextOut(hdc, 700, 0, wstr.c_str(), wstr.length() + 1);
}

void GoodView::Message(string text)
{
	SelectObject(hdc, backBrush);
	Rectangle(hdc, 50, 600, 1150, 650);

	wstring wstr = s2ws(text);
	SelectObject(hdc, hfont);
	TextOut(hdc, 50, 600, wstr.c_str(), wstr.length() + 1);
}

void GoodView::ChangePlayer()
{
	system("CLS");
	Message("GIVE CONTROLLS TO ANOTHER PLAYER AND PRESS ENTER");
}