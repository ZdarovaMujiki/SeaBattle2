#ifndef GAMER_H
#define GAMER_H
#include "GameView.h"

int press();

class IGamer
{
public:
	virtual void GameStage() = 0;
	virtual int Attack(int x, int y) = 0;
	virtual void SetEnemy(IGamer* player) = 0;
	virtual void SetView(GameView* view) = 0;
	virtual void PrepareStage() = 0;
};

class ConsoleGamer : public IGamer
{
private:
	string name;
	IGamer* enemy;
	GameView* graphic;
	int killCount = 0, turn;
	char** field, ** attackField;
	bool** mask;
	pair<int, int> cursor;
	ofstream logs;
public:
	ConsoleGamer(string str, IGamer* player);
	void GameStage() override;
	int Attack(int x, int y) override;
	void KillConfirm(int x, int y, char** field);
	void SetEnemy(IGamer* player) override;
	bool placeShip(int shipSize, rotation rot);
	void makeMask(int shipSize, rotation rot);
	void PrepareStage() override;
	void SetView(GameView* view) override;
	bool MoveCursor(int key, int currentShip, rotation rot);
};

class RandomGamer : public  IGamer
{
private:
	string name;
	char** field, ** attackField;
	bool** mask;
	IGamer* enemy;
	GameView* graphic;
	int killCount = 0, turn;
	pair<int, int> cursor;
	ofstream logs;
public:
	RandomGamer(string str, IGamer* player);
	void GameStage() override;
	void KillConfirm(int x, int y, char** field);
	int Attack(int x, int y) override;
	void SetEnemy(IGamer* player) override;
	bool placeShip(int shipSize, rotation rot);
	void makeMask(int shipSize, rotation rot);
	void PrepareStage() override;
	void SetView(GameView* view) override;
};

class OptiomalGamer : public IGamer
{
private:
	string name;
	char** field, ** attackField;
	bool** mask;
	IGamer* enemy;
	GameView* graphic;
	int killCount = 0, turn;
	pair<int, int> cursor;
	pair<int, int> target = { 10, 10 };
	pair<int, int> firstHit = { 10, 10 };
	ofstream logs;
public:
	OptiomalGamer(string str, IGamer* player);
	void GameStage() override;
	void KillConfirm(int x, int y, char** field);
	int Attack(int x, int y) override;
	void SetEnemy(IGamer* player) override;
	pair<int, int> AcquireTarget(int x, int y);
	bool placeShip(int shipSize, rotation rot);
	void makeMask(int shipSize, rotation rot);
	void PrepareStage() override;
	void SetView(GameView* view) override;
};
#endif GAMER_H