#include "gamer.h"

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	srand(time(0));
	int roundAmount = 1;
	IGamer *playerA = nullptr, *playerB = nullptr;
	GameView* view = nullptr;

	argc = 9;
	argv[1] = (char*)"-f";
	argv[2] = (char*)"0";
	argv[3] = (char*)"-s";
	argv[4] = (char*)"2";
	argv[5] = (char*)"-c";
	argv[6] = (char*)"10";
	argv[7] = (char*)"-g";
	argv[8] = (char*)"1";
	argv[8] = (char*)"high";

	for (int i = 1; i < argc; ++i)
	{
		if ((string)argv[i] == "-h" || (string)argv[i] == "--help")
		{
			cout << "SEA BATTLE"
				<< endl << "AVAILABLE COMMANDS:"
				<< endl << "[-h | --help] - help message"
				<< endl << "[-c | --count] [round_amount] - set round amount"
				<< endl << "[-f | --first] [console (0) | random (1) | optimal (2)]- set first player type"
				<< endl << "[-s | --second] [console (0) | random (1) | optimal (2)] - set second player type"
				<< endl << "[-g | --graphic] [low (0) | high (1)] - set graphic level"
				<< endl << "console controls:"
				<< endl << "arrow keys - move cursor"
				<< endl << "space - place ship / attack"
				<< endl << "r - rotate ship"
				<< endl << "tab - restart"
				<< endl;
			return 0;
		}
		else if ((string)argv[i] == "-c" || (string)argv[i] == "--cout")
		{
			roundAmount = atoi(argv[++i]);
		}
		else if ((string)argv[i] == "-f" || (string)argv[i] == "--first")
		{
			if ((string)argv[i + 1] == "console" || (string)argv[i + 1] == "0")
			{
				playerA = new ConsoleGamer("PLAYER A", playerB);
			}
			else if ((string)argv[i + 1] == "random" || (string)argv[i + 1] == "1")
			{
				playerA = new RandomGamer("PLAYER A", playerB);
			}
			else if ((string)argv[i + 1] == "optimal" || (string)argv[i + 1] == "2")
			{
				playerA = new OptiomalGamer("PLAYER A", playerB);
			}
			++i;
		}
		else if ((string)argv[i] == "-s" || (string)argv[i] == "--second")
		{
			if ((string)argv[i + 1] == "console" || (string)argv[i + 1] == "0")
			{
				playerB = new ConsoleGamer("PLAYER B", playerA);
			}
			else if ((string)argv[i + 1] == "random" || (string)argv[i + 1] == "1")
			{
				playerB = new RandomGamer("PLAYER B", playerA);
			}
			else if ((string)argv[i + 1] == "optimal" || (string)argv[i + 1] == "2")
			{
				playerB = new OptiomalGamer("PLAYER B", playerA);
			}
			++i;
		}
		else if ((string)argv[i] == "-g" || (string)argv[i] == "--graphic")
		{
			if ((string)argv[i + 1] == "low" || (string)argv[i + 1] == "0")
			{
				view = new ConsoleView;
			}
			else if ((string)argv[i + 1] == "high" || (string)argv[i + 1] == "1")
			{
				view = new GoodView;
			}
			++i;
		}
	}
	if (view == nullptr)
	{
		view = new ConsoleView;
	}
	if (playerA == nullptr)
	{
		playerA = new RandomGamer("PLAYER A", playerB);
	}
	if (playerB == nullptr)
	{
		playerB = new RandomGamer("PLAYER B", playerA);
	}
	playerA->SetEnemy(playerB);
	playerA->SetView(view);
	playerB->SetEnemy(playerA);
	playerB->SetView(view);

	for (int i = 0; i < roundAmount; ++i)
	{
		system("CLS");
		view->Message("ROUND " + to_string(i + 1), "PREPARE TO FIGHT!");
		_getch();
		system("CLS");
		playerA->PrepareStage();
		playerB->PrepareStage();
		playerA->GameStage();
	}

	return 0;
}