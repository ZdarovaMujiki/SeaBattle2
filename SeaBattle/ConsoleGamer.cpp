#include "gamer.h"

int press()
{
	int key = _getch();
	if (key == 224)
	{
		return _getch();
	}
	return key;
}

ConsoleGamer::ConsoleGamer(string str, IGamer* player) : name(str)
{
	logs.open(name + ".txt");
	enemy = player;
}

void ConsoleGamer::PrepareStage()
{
	logs << endl << "ROUND" << endl;
	turn = name == (string)"PLAYER A" ? -1 : 0;

	killCount = 0;
	mask = new bool* [10]();
	field = new char* [10]();
	attackField = new char* [10]();
	for (int i = 0; i < 10; ++i)
	{
		mask[i] = new bool[10]();
		field[i] = new char[10]();
		attackField[i] = new char[10]();
		for (int j = 0; j < 10; ++j)
		{
			attackField[i][j] = 'O';
			field[i][j] = 'O';
		}
	}
	cursor = make_pair(0, 0);

	rotation rot = hor;
	map<int, int> fleet;
	int currentShip = 1;
	int key = 0;

	for (int i = 1; i < 5; ++i)
	{
		fleet.insert(make_pair(i, 5 - i));
	}
	while (key != 13 && fleet[4])
	{
		graphic->Draw(rot, cursor.first, cursor.second, currentShip, field, fleet, name);
		key = _getch();

		if (!MoveCursor(key, currentShip, rot))
		{
			if (key == 114 || key == 170) //rotate (R)
			{
				if ((rot == vert || cursor.second <= 9 - currentShip + 1) && (rot == hor || cursor.first <= 9 - currentShip + 1))
				{
					rot = static_cast<rotation>(rot ^ 1);
				}
			}
			else if (key == 32)	//place (space)
			{
				if (placeShip(currentShip, rot))
				{
					--fleet[currentShip];
					if (fleet[currentShip] == 0)
					{
						++currentShip;
					}
				}
			}
			else if (key == 9)	//reload (tab)
			{
				PrepareStage();
				return;
			}
		}
	}
	system("CLS");
	graphic->Message("THIS IS YOUR FIELD, PRESS ENTER");
	graphic->Draw(field);
	while (press() != 13);
	system("CLS");
	if (dynamic_cast<ConsoleGamer*>(enemy))
	{
		graphic->ChangePlayer();
		while (press() != 13);
	}
}
void ConsoleGamer::GameStage()
{
	turn += 2;
	logs << "TURN ¹" << turn << endl;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			logs << field[i][j];
		}
		logs << endl;
	}
	int key = 0;

	while (killCount != 10)
	{
		graphic->Draw(cursor.first, cursor.second, field, attackField, name);
		key = _getch();
		if (!MoveCursor(key, 1, hor))
		{
			if (key == 32 && attackField[cursor.second][cursor.first] == 'O')
			{
				logs << to_string(cursor.first) << ":" << to_string(cursor.second) << endl;
				int code = enemy->Attack(cursor.first, cursor.second);
				if (code == 0) //miss
				{
					attackField[cursor.second][cursor.first] = '0';
					logs << endl;

					graphic->Message("YOU MISS, PRESS ANY BUTTON");
					_getch();

					if (dynamic_cast<ConsoleGamer*>(enemy))
					{
						system("CLS");
						graphic->ChangePlayer();
						while (press() != 13);
					}
					enemy->GameStage();
					return;
				}
				else if (code == 1) //hit
				{
					attackField[cursor.second][cursor.first] = '@';
					graphic->Message("YOU HIT, PRESS ANY BUTTON");
				}
				else if (code == 2) //kill
				{
					attackField[cursor.second][cursor.first] = '@';
					KillConfirm(cursor.first, cursor.second, attackField);
					++killCount;
					graphic->Message("YOU KILL, PRESS ANY BUTTON");
				}
				_getch();
			}
		}
	}
	system("CLS");
	graphic->Draw(field, attackField, name);
	graphic->Message(name + " WIN! PRESS ENTER");
	while (press() != 13);
}
void ConsoleGamer::KillConfirm(int x, int y, char** field)
{
	rotation rot;
	if ((y - 1 >= 0 && field[y - 1][x] == '@') || (y + 1 < 10 && field[y + 1][x] == '@'))
	{
		rot = vert;
		while (y - 1 >= 0 && field[y - 1][x] == '@')
		{
			--y;
		}
	}
	else
	{
		rot = hor;
		while (x - 1 >= 0 && field[y][x - 1] == '@')
		{
			--x;
		}
	}

	while (y < 10 && x < 10 && field[y][x] == '@')
	{
		for (int i = -1; i <= 1; ++i)
		{
			if (x + i >= 0 && x + i < 10)
			{
				if (y - 1 >= 0 && field[y - 1][x + i] != '@')
				{
					field[y - 1][x + i] = '0';
				}
				if (field[y][x + i] != '@')
				{
					field[y][x + i] = '0';
				}
				if (y + 1 < 10 && field[y + 1][x + i] != '@')
				{
					field[y + 1][x + i] = '0';
				}
			}
		}
		rot == vert ? ++y : ++x;
	}
}
int ConsoleGamer::Attack(int x, int y)
{
	if (field[y][x] == '#') //on ship
	{
		rotation rot;
		field[y][x] = '@';
		if ((y - 1 >= 0 && (field[y - 1][x] == '@' || field[y - 1][x] == '#')) || (y + 1 < 10 && (field[y + 1][x] == '@' || field[y + 1][x] == '#')))
		{
			rot = vert;
			while (y - 1 >= 0 && (field[y - 1][x] == '@' || field[y - 1][x] == '#'))
			{
				--y;
			}
		}
		else
		{
			rot = hor;
			while (x - 1 >= 0 && (field[y][x - 1] == '@' || field[y][x - 1] == '#'))
			{
				--x;
			}
		}

		while (y < 10 && x < 10 && (field[y][x] == '@' || field[y][x] == '#'))
		{
			if (field[y][x] == '#')
			{
				return 1;  //hit
			}
			rot == vert ? ++y : ++x;
		}
		rot == vert ? --y : --x;
		KillConfirm(x, y, field);
		return 2; //kill
	}
	field[y][x] = '0'; //miss
	return 0;
}
void ConsoleGamer::SetEnemy(IGamer* player)
{
	enemy = player;
}
void ConsoleGamer::SetView(GameView* view)
{
	graphic = view;
}
bool ConsoleGamer::placeShip(int shipSize, rotation rot)
{
	int x = cursor.first;
	int y = cursor.second;
	if (rot == hor)
	{
		for (int i = 0; i < shipSize; ++i)
		{
			if (x + i > 9 || mask[y][x + i] == 1)
			{
				return 0;
			}
		}
		for (int i = 0; i < shipSize; ++i)
		{
			field[y][x + i] = '#';
		}
	}
	else // rot == vert
	{
		for (int i = 0; i < shipSize; ++i)
		{
			if (y + i > 9 || mask[y + i][x] == 1)
			{
				return 0;
			}
		}
		for (int i = 0; i < shipSize; ++i)
		{
			field[y + i][x] = '#';
		}
	}
	makeMask(shipSize, rot);
	return 1;
}
void ConsoleGamer::makeMask(int shipSize, rotation rot)
{
	int x = cursor.first;
	int y = cursor.second;
	if (rot == hor)
	{
		for (int i = -1; i <= shipSize; ++i)
		{
			if (x + i < 10 && x + i >= 0)
			{
				mask[y][x + i] = 1;
				if (y - 1 >= 0)
				{
					mask[y - 1][x + i] = 1;
				}
				if (y + 1 < 10)
				{
					mask[y + 1][x + i] = 1;
				}
			}
		}
	}
	else // rot == vert
	{
		for (int i = -1; i <= shipSize; ++i)
		{
			if (y + i < 10 && y + i >= 0)
			{
				mask[y + i][x] = 1;
				if (x - 1 >= 0)
				{
					mask[y + i][x - 1] = 1;
				}
				if (x + 1 < 10)
				{
					mask[y + i][x + 1] = 1;
				}
			}
		}
	}
}

bool ConsoleGamer::MoveCursor(int key, int currentShip, rotation rot)
{
	if (key == 77 && (cursor.first + currentShip - 1 < 9 || (rot == vert && cursor.first != 9)))
	{
		++cursor.first;
		return 1;
	}
	else if (key == 75 && cursor.first != 0)
	{
		--cursor.first;
		return 1;
	}
	else if (key == 80 && (cursor.second + currentShip - 1 < 9 || (rot == hor && cursor.second != 9)))
	{
		++cursor.second;
		return 1;
	}
	else if (key == 72 && cursor.second != 0)
	{
		--cursor.second;
		return 1;
	}
	return 0;
}