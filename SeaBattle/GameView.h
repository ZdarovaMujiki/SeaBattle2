#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <wincon.h>
#include <d2d1.h>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <time.h>

using namespace std;
enum rotation { hor, vert };

class GameView
{
public:
	virtual void Draw(rotation rot, int x, int y, int currentShip, char **field, map<int, int> fleet, string name) = 0;
	virtual void Draw(char** field) = 0;
	virtual void Draw(char** field, char** attackField, string name) = 0;
	virtual void Draw(int x, int y, char** field, char** attackField, string name) = 0;
	virtual void Message(string text1, string text2) = 0;
	virtual void Message(string text) = 0;
	virtual void ChangePlayer() = 0;
};

class ConsoleView : public GameView
{
public:
	void Draw(rotation rot, int x, int y, int currentShip, char **field, map<int, int> fleet, string name) override;
	void Draw(char** field) override;
	void Draw(char** field, char** attackField, string name) override;
	void Draw(int x, int y, char** field, char** attackField, string name) override;
	void Message(string text1, string text2) override;
	void ChangePlayer() override;
	void Message(string text) override;
};

class GoodView : public GameView
{
private:
	HDC hdc;
	
	HFONT hfont;

	HBRUSH blueBrush;
	HBRUSH darkBlueBrush;
	HBRUSH whiteBrush;
	HBRUSH redBrush;
	HBRUSH backBrush;
	HBRUSH vertBrush;
	HBRUSH horBrush;
	HBRUSH crossBrush;

	HPEN whitePen;
	HPEN blackPen;
public:
	GoodView();
	void Draw(rotation rot, int x, int y, int currentShip, char **field, map<int, int> fleet, string name) override;
	void Draw(char** field) override;
	void Draw(char** field, char** attackField, string name) override;
	void Draw(int x, int y, char** field, char** attackField, string name) override;
	void Message(string text1, string text2) override;
	void ChangePlayer() override;
	void Message(string text) override;
};
#endif GAMEVIEW_H