#include "gamer.h"

OptiomalGamer::OptiomalGamer(string str, IGamer* player) : name(str)
{
	logs.open(name + ".txt");
	enemy = player;
}
void OptiomalGamer::PrepareStage()
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
	target.first = 10;
	firstHit.first = 10;

	rotation rot;
	map<int, int> fleet;
	int currentShip = 1;

	for (int i = 1; i < 5; ++i)
	{
		fleet.insert(make_pair(i, 5 - i));
	}
	for (int tryCount = 0; currentShip != 5; ++tryCount)
	{
		rot = static_cast<rotation>(rand() % 2);
		cursor = make_pair(rand() % 10, rand() % 10);

		if (placeShip(currentShip, rot))
		{
			--fleet[currentShip];
			if (fleet[currentShip] == 0)
			{
				++currentShip;
			}
		}
		else if (tryCount > 300)
		{
			PrepareStage();
			return;
		}
	}
	system("CLS");
}
void OptiomalGamer::GameStage()
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
	
	while (killCount != 10)
	{
		//have target or no target
		cursor = target.first != 10 ? target : make_pair(rand() % 10, rand() % 10);

		if (attackField[cursor.second][cursor.first] == 'O')
		{
			logs << cursor.first << ":" << cursor.second << endl;
			int code = enemy->Attack(cursor.first, cursor.second);
			if (code == 0) //miss
			{
				attackField[cursor.second][cursor.first] = '0';
				logs << endl;
				if (target.first != 10) //have target
				{
					target = AcquireTarget(firstHit.first, firstHit.second);
				}

				enemy->GameStage();
				return;
			}
			else if (code == 1) //hit
			{
				attackField[cursor.second][cursor.first] = '@';
				if (target.first != 10) //have target
				{
					int deltaX = target.first - firstHit.first;
					if (deltaX)
					{
						if (target.second - 1 >= 0)
						{
							attackField[target.second - 1][target.first] = '0';
							attackField[firstHit.second - 1][firstHit.first] = '0';
						}
						if (target.second + 1 <= 9)
						{
							attackField[target.second + 1][target.first] = '0';
							attackField[firstHit.second + 1][firstHit.first] = '0';
						}
					}
					else
					{
						if (target.first - 1 >= 0)
						{
							attackField[target.second][target.first - 1] = '0';
							attackField[firstHit.second][firstHit.first - 1] = '0';
						}
						if (target.first + 1 <= 9)
						{
							attackField[target.second][target.first + 1] = '0';
							attackField[firstHit.second][firstHit.first + 1] = '0';
						}
					}
					target = AcquireTarget(target.first, target.second);
					if (target.first == 10)
					{
						target = AcquireTarget(firstHit.first, firstHit.second);
					}
				}
				else //no target
				{
					firstHit = cursor;
					target = AcquireTarget(firstHit.first, firstHit.second);
				}
			}
			else if (code == 2) //kill 
			{
				attackField[cursor.second][cursor.first] = '@';
				KillConfirm(cursor.first, cursor.second, attackField);
				++killCount;
			
				target.first = 10;
				firstHit.first = 10;
			}
		}
	}
	system("CLS");
	graphic->Draw(field, attackField, name);
	graphic->Message(name + " WIN! PRESS ENTER");
	while (press() != 13);
}
void OptiomalGamer::KillConfirm(int x, int y, char **field)
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
int OptiomalGamer::Attack(int x, int y)
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
void OptiomalGamer::SetEnemy(IGamer* player)
{
	enemy = player;
}
void OptiomalGamer::SetView(GameView* view)
{
	graphic = view;
}
bool OptiomalGamer::placeShip(int shipSize, rotation rot)
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
void OptiomalGamer::makeMask(int shipSize, rotation rot)
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
pair<int, int> OptiomalGamer::AcquireTarget(int x, int y)
{
	int strikeOrder[4] = {0, 1, 2, 3};
	swap(strikeOrder[rand() % 4], strikeOrder[rand() % 4]);
	swap(strikeOrder[rand() % 4], strikeOrder[rand() % 4]);
	
	for (int i = 0; i <= 3; ++i)
	{
		switch (strikeOrder[i])
		{
		case 0:
		{
			if (y - 1 >= 0 && attackField[y - 1][x] == 'O')
			{
				return make_pair(x, y - 1);
			}
			break;
		}
		case 1:
		{
			if (x + 1 <= 9 && attackField[y][x + 1] == 'O')
			{
				return make_pair(x + 1, y);
			}
			break;
		}
		case 2:
		{
			if (y + 1 <= 9 && attackField[y + 1][x] == 'O')
			{
				return make_pair(x, y + 1);
			}
			break;
		}
		case 3:
		{
			if (x - 1 >= 0 && attackField[y][x - 1] == 'O')
			{
				return make_pair(x - 1, y);
			}
			break;
		}
		}
	}
	return make_pair(10, 10);
}