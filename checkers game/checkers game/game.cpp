#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;
const int rows = 8, cols = 8;
void gameprocessing();
void intialisation(char **);
void displaygrid(char **, int, int);
void validate(char **, int &, int &, int &, int &, char, int &);
void checkposition(int &, int &);
void jump(char **, int &, int &, int &, char);
void computer(char **, int, int, int &);
void computerattack(char **, int, int, int&);
void computerothermoves(char **, int &);
void computerking(char **, int, int, int &, bool &);
int main()
{
	srand(time(NULL));
	gameprocessing();
	system("pause");
}
void gameprocessing()
{
	int rposition, cposition, rmove, cmove, opponent = 12, user = 12, turn = 0;
	char **grid = new char *[rows];
	for (int i = 0; i < rows; i++)
		grid[i] = new char[cols];
	intialisation(grid);
	displaygrid(grid, opponent, user);
	while (opponent != 0 && user != 0)
	{
		if (turn % 2 == 0)
		{
			cout << "Player 1's turn\n";
			cout << "Enter row and column number of check do you want to select: ";
			cin >> rposition >> cposition;
			checkposition(rposition, cposition);
			validate(grid, rposition, cposition, rmove, cmove, '1', opponent);
			Sleep(1500);
			displaygrid(grid, opponent, user);
		}
		else
		{
			cout << "Player 0's turn\n";
			Sleep(3000);
			computer(grid, rmove, cmove, user);
			displaygrid(grid, opponent, user);
		}
		turn++;
		if (opponent == 0)
			cout << "User wins the game\n";
		else if (user == 0)
			cout << "Opponent wins the game\n";
	}
	for (int i = 0; i < rows; i++)
		delete[]grid[i];
	delete[]grid;
}
void intialisation(char **grid)
{
	for (int i = 0; i < rows; i++)
	{
		if (i >= 0 && i < 3)
			for (int j = 0; j < cols; j++)
				if ((i + j) % 2 == 0)
					*(grid[i] + j) = ' ';
				else
					*(grid[i] + j) = '0';
		if (i>2 && i < 5)
			for (int j = 0; j < cols; j++)
				if ((i + j) % 2 == 0)
					*(grid[i] + j) = ' ';
				else
					*(grid[i] + j) = '*';
		if (i > 4)
			for (int j = 0; j < cols; j++)
				if ((i + j) % 2 != 0)
					*(grid[i] + j) = '1';
				else
					*(grid[i] + j) = ' ';
	}
}
void displaygrid(char **grid, int opponent, int user)
{
	system("cls");
	system("COLOR 0E");
	cout << "\n\n" << setw(26) << "user = " << user << "    opponent= " << opponent << "\n\n";
	cout << setw(31);
	for (int j = 0; j < cols; j++)
		cout << j << " | ";
	cout << "\n" << setw(33);
	for (int i = 0; i < cols; i++)
		cout << "----";
	cout << "\n";
	for (int i = 0; i < rows; i++)
	{
		cout << setw(27) << i << " | ";
		for (int j = 0; j < cols; j++)
			cout << *(grid[i] + j) << " | ";
		cout << "\n" << setw(33);
		for (int i = 0; i < cols; i++)
			cout << "----";
		cout << endl;
	}

}
void validate(char **grid, int &rposition, int &cposition, int &rmove, int &cmove, char player, int &opponent)
{
	/*continue taking check position until user enter a valid check position*/
	while (*(grid[rposition] + cposition) != player&&grid[rposition][cposition] != 'P')
	{
		cout << "Not a valid check selected ,Select a valid row and column position of your check: ";
		cin >> rposition >> cposition;
		checkposition(rposition, cposition);
	}
	cout << "Enter row and column position of your move: ";
	cin >> rmove >> cmove;
	checkposition(rmove, cmove);
	/*continue taking move position until user enter a valid move position*/
	bool check = false;
	while (*(grid[rmove] + cmove) != '*'&& check == false)
	{
		cout << "Not a valid move selected ,Select a valid row and column position of your move: ";
		cin >> rmove >> cmove;
		checkposition(rmove, cmove);
		if (*(grid[rposition] + cposition) == '1'&&rmove < rposition&&cmove != cposition)
			check = true;
		/*if user selects a king check then allow backward move valid*/
		else if (*(grid[rposition] + cposition) == 'P' && (rmove > rposition || rmove < rposition) && cmove != cposition)
			check = true;
	}
	/*checks if user want to kill opponents check then remove it*/
	if ((rposition - 2 >= 0) && ((cposition + 2 < cols && (*(grid[rposition - 1] + cposition + 1) == '0' || *(grid[rposition - 1] + cposition + 1) == 'C') && *(grid[rposition - 2] + cposition + 2) == '*') || (cposition - 2 >= 0 && (*(grid[rposition - 1] + cposition - 1) == '0' || *(grid[rposition - 1] + cposition - 1) == 'C') && *(grid[rposition - 2] + cposition - 2) == '*')))
	{
		if (*(grid[rposition - 1] + cposition + 1) == '0' || *(grid[rposition - 1] + cposition + 1) == 'C')
			*(grid[rposition - 1] + cposition + 1) = '*';
		else
			*(grid[rposition - 1] + cposition - 1) = '*';
		if (*(grid[rposition] + cposition) == 'P')
			*(grid[rmove] + cmove) = 'P';
		else
			*(grid[rmove] + cmove) = player;
		*(grid[rposition] + cposition) = '*';
		opponent--;
		if (rmove == 0)
		{
			grid[rmove][cmove] = 'P';
			jump(grid, rmove, cmove, opponent, 'P');
		}
		else
			jump(grid, rmove, cmove, opponent, '1');
	}
	/*checks if user want to kill with kill with king then make backward move and kill it*/
	else if (*(grid[rposition] + cposition) == 'P'&&rposition + 2 < rows && ((cposition - 2 >= 0 && (*(grid[rposition + 1] + cposition - 1) == '0' || *(grid[rposition + 1] + cposition - 1) == 'C') && *(grid[rposition + 2] + cposition - 2) == '*') || (cposition + 2 < cols && (*(grid[rposition + 1] + cposition + 1) == '0' || *(grid[rposition + 1] + cposition + 1) == 'C') && *(grid[rposition + 2] + cposition + 2) == '*')))
	{

		if (*(grid[rposition + 1] + cposition - 1) == '0' || *(grid[rposition + 1] + cposition - 1) == 'C')
			*(grid[rposition + 1] + cposition - 1) = '*';
		else
			*(grid[rposition + 1] + cposition + 1) = '*';
		*(grid[rmove] + cmove) = 'P';
		*(grid[rposition] + cposition) = '*';
		opponent--;
		jump(grid, rmove, cmove, opponent, 'P');
	}
	else
	{
		if (*(grid[rposition] + cposition) == 'P')
			*(grid[rmove] + cmove) = 'P';
		else
			*(grid[rmove] + cmove) = player;
		*(grid[rposition] + cposition) = '*';
		if (rmove == 0)
			*(grid[rmove] + cmove) = 'P';
	}
}
void checkposition(int &rposition, int &cposition)
{
	while (rposition < 0 || rposition>rows - 1 || cposition < 0 || cposition>cols - 1)
	{
		cout << "Not a valid row or column position,Select a valid row and column position\n";
		cin >> rposition >> cposition;
	}
}
void jump(char **grid, int &rmove, int &cmove, int &opponent, char player)
{
	if ((rmove - 2 >= 0) && ((cmove + 2 < cols && (*(grid[rmove - 1] + cmove + 1) == '0' || *(grid[rmove - 1] + cmove + 1) == 'C') && *(grid[rmove - 2] + cmove + 2) == '*') || (cmove - 2 >= 0 && (*(grid[rmove - 1] + cmove - 1) == '0' || *(grid[rmove - 1] + cmove - 1) == 'C') && *(grid[rmove - 2] + cmove - 2) == '*')))
	{
		if (cmove + 2 < cols && (*(grid[rmove - 1] + cmove + 1) == '0' || *(grid[rmove - 1] + cmove + 1) == 'C'))
		{
			*(grid[rmove - 1] + cmove + 1) = '*';
			*(grid[rmove] + cmove) = '*';
			*(grid[rmove - 2] + cmove + 2) = player;
			if (rmove - 2 == 0)
				*(grid[rmove - 2] + cmove + 2) = 'P';
			cmove = cmove + 2;
		}
		else
		{
			*(grid[rmove - 1] + cmove - 1) = '*';
			*(grid[rmove] + cmove) = '*';
			*(grid[rmove - 2] + cmove - 2) = player;
			if (rmove - 2 == 0)
				*(grid[rmove - 2] + cmove - 2) = 'P';
			cmove = cmove - 2;
		}
		rmove = rmove - 2;
		opponent--;
	}
	else if (player == 'P' && ((rmove + 2 < rows) && ((cmove + 2 < cols && (grid[rmove + 1][cmove + 1] == '0' || grid[rmove + 1][cmove + 1] == 'C') && grid[rmove + 2][cmove + 2] == '*') || (cmove - 2 >= 0 && (grid[rmove + 1][cmove - 1] == '0' || grid[rmove + 1][cmove - 1] == 'C') && grid[rmove + 2][cmove - 2] == '*'))))
	{
		if (grid[rmove + 1][cmove + 1] == '0' || grid[rmove + 1][cmove + 1] == 'C')
		{
			grid[rmove + 1][cmove + 1] = '*';
			grid[rmove][cmove] = '*';
			grid[rmove + 2][cmove + 2] = 'P';
			cmove = cmove + 2;
		}
		else
		{
			grid[rmove + 1][cmove - 1] = '*';
			grid[rmove][cmove] = '*';
			grid[rmove + 2][cmove - 2] = 'P';
			cmove = cmove - 2;
		}
		rmove = rmove + 2;
		opponent--;
	}
}
void computer(char **grid, int rmove, int cmove, int &user)
{
	bool done = false;
	/*first check the last move user has made and kill the check of the opponent*/
	if ((rmove - 1 >= 0 && rmove + 1 < rows&&cmove - 1 >= 0 && cmove + 1 < cols) && (((grid[rmove - 1][cmove + 1] == '0' || grid[rmove - 1][cmove + 1] == 'C') && grid[rmove + 1][cmove - 1] == '*') || (grid[rmove - 1][cmove - 1] == '0'&& grid[rmove + 1][cmove + 1] == '*')) && grid[rmove][cmove] == '1' || (grid[rmove + 1][cmove + 1] == 'C'&&grid[rmove - 1][cmove - 1] == '*') || (grid[rmove + 1][cmove - 1] == 'C'&&grid[rmove - 1][cmove + 1] == '*'))
		if ((grid[rmove + 1][cmove + 1] == 'C'&&grid[rmove - 1][cmove - 1] == '*') || (grid[rmove + 1][cmove - 1] == 'C'&&grid[rmove - 1][cmove + 1] == '*'))
			computerking(grid, rmove, cmove, user, done);
		else
			computerattack(grid, rmove, cmove, user);
	else
	{
		computerothermoves(grid, user);
	}
}
void computerattack(char **grid, int rmove, int cmove, int &user)
{
	bool done = false;
	/*kill's user check by comparing user last move*/
	if (((grid[rmove - 1][cmove + 1] == '0' || grid[rmove - 1][cmove + 1] == 'C') && grid[rmove + 1][cmove - 1] == '*') && (grid[rmove][cmove] == '1'))      /*move the check to lower left position*/
	{
		grid[rmove - 1][cmove + 1] = '*';
		grid[rmove][cmove] = '*';
		if (grid[rmove - 1][cmove + 1] == 'C')
			grid[rmove + 1][cmove - 1] = 'C';
		else
			grid[rmove + 1][cmove - 1] = '0';
		if (rmove + 1 >= rows - 1)
			grid[rmove + 1][cmove - 1] = 'C';
		user--;
	}
	else if (((grid[rmove - 1][cmove - 1] == '0' || grid[rmove - 1][cmove - 1] == 'C') && grid[rmove + 1][cmove + 1] == '*') && (grid[rmove][cmove] == '1'))      /*move the check to lower right position*/
	{
		if (grid[rmove - 1][cmove - 1] == 'C')
			grid[rmove + 1][cmove + 1] = 'C';
		else
			grid[rmove + 1][cmove + 1] = '0';
		grid[rmove - 1][cmove - 1] = '*';
		grid[rmove][cmove] = '*';
		if (rmove + 1 >= rows - 1)
			grid[rmove + 1][cmove + 1] = 'C';
		user--;
	}
	else if (grid[rmove - 1][cmove + 1] == 'C')
		computerking(grid, rmove - 1, cmove + 1, user, done);
	else if (grid[rmove - 1][cmove - 1] == 'C')
		computerking(grid, rmove - 1, cmove - 1, user, done);
	/*check is there any other check to remove after opponents move if it is then call the function again*/
	if ((rmove + 3 >= 0 && rmove + 3 < rows && cmove + 1 >= 0 && cmove + 1 < cols) && (grid[rmove + 2][cmove] == '1'&&grid[rmove + 3][cmove - 1] == '*'))                    /*Repeat if there is users check in lower left diagonal*/
		computerattack(grid, rmove + 2, cmove, user);
	else if ((rmove + 3 >= 0 && rmove + 3 < rows && cmove - 2 >= 0) && grid[rmove + 2][cmove - 2] == '1'&&grid[rmove + 3][cmove - 3] == '*')
		computerattack(grid, rmove + 2, cmove - 2, user);
	else if ((rmove + 3 >= 0 && rmove + 3 < rows && cmove + 3 < cols) && grid[rmove + 2][cmove + 2] == '1'&&grid[rmove + 3][cmove + 3] == '*')
		computerattack(grid, rmove + 2, cmove + 2, user);
}
void computerothermoves(char **grid, int &user)
{
	int r_comp, c_comp;
	/*if there is no check to kill from users last move then generates a random number to choose the check of opponent and move it*/
	bool done = false;
	r_comp = rand() % rows;
	c_comp = rand() % cols;
	while (done != true)
	{
		r_comp = rand() % rows;
		c_comp = rand() % cols;
		if (grid[r_comp][c_comp] == '0' || grid[r_comp][c_comp] == 'C')
		{
			if (grid[r_comp][c_comp] == 'C')
				computerking(grid, r_comp, c_comp, user, done);
			else if (r_comp + 2 < rows && ((c_comp - 2 >= 0 && (grid[r_comp + 1][c_comp - 1] == '1' || grid[r_comp + 1][c_comp - 1] == 'P')) || (c_comp + 2 < cols && (grid[r_comp + 1][c_comp + 1] == '1' || grid[r_comp + 1][c_comp + 1] == 'P'))))
			{
				if (r_comp + 2 < rows&&c_comp - 2 >= 0 && (grid[r_comp + 1][c_comp - 1] == '1' || grid[r_comp + 1][c_comp - 1] == 'P') && grid[r_comp + 2][c_comp - 2] == '*')
				{                 /*Makes move in the lower left diagonal*/
					grid[r_comp + 2][c_comp - 2] = '0';
					grid[r_comp][c_comp] = '*';
					grid[r_comp + 1][c_comp - 1] = '*';
					if (r_comp + 2 == rows - 1)
						grid[r_comp + 2][c_comp - 2] = 'C';
					user--;
					done = true;
				}
				else if ((r_comp + 2 < rows&&c_comp + 2 < cols) && (grid[r_comp + 1][c_comp + 1] == '1' || grid[r_comp + 1][c_comp + 1] == 'P') && grid[r_comp + 2][c_comp + 2] == '*')
				{                   /*Makes move in the lower right diagonal*/
					grid[r_comp + 2][c_comp + 2] = '0';
					grid[r_comp][c_comp] = '*';
					grid[r_comp + 1][c_comp + 1] = '*';
					if (r_comp + 2 == rows - 1)
						grid[r_comp + 2][c_comp + 2] = 'C';
					user--;
					done = true;
				}
			}
			else if (r_comp + 2 < rows && ((c_comp - 2 >= 0 && grid[r_comp + 2][c_comp - 2] == '*') || (c_comp + 2 < cols&&grid[r_comp + 2][c_comp + 2] == '*')))  
			{
				if (grid[r_comp + 2][c_comp - 2] == '*')
				{
					grid[r_comp + 2][c_comp - 2] = '0';
					grid[r_comp][c_comp] = '*';
					if (r_comp + 2 == rows - 1)
						grid[r_comp + 2][c_comp - 2] = 'C';
					done = true;
				}
				else if (grid[r_comp + 2][c_comp + 2] == '*')
				{   
					grid[r_comp + 2][c_comp + 2] = '0';
					grid[r_comp][c_comp] = '*';
					if (r_comp + 2 == rows - 1)
						grid[r_comp + 2][c_comp + 2] = 'C';
					done = true;
				}
			}
		}
	}
}
void computerking(char **grid, int r_comp, int c_comp, int &user, bool &done)
{
	/*it makes a backward move if opponent is a king*/
	/*These conditions work when user move is passed in the function:-
	Check whether user check can be killed by checking it with user's last move*/
	if ((r_comp + 1 < rows&&r_comp - 1 >= 0 && c_comp + 1 < cols&&c_comp - 1 >= 0) && grid[r_comp + 1][c_comp + 1] == 'C'&&grid[r_comp - 1][c_comp - 1] == '*')    /*Makes move in the upper left diagonal*/
	{
		grid[r_comp + 1][c_comp + 1] = '*';
		grid[r_comp - 1][c_comp - 1] = 'C';
		grid[r_comp][c_comp] = '*';
		user--;
		done = true;
	}
	else if ((r_comp + 1 < rows&&r_comp - 1 >= 0 && c_comp + 1 < cols&&c_comp - 1 >= 0) && grid[r_comp + 1][c_comp - 1] == 'C'&&grid[r_comp - 1][c_comp + 1] == '*')    /*Makes move in the upper right diagonal*/
	{
		grid[r_comp + 1][c_comp - 1] = '*';
		grid[r_comp - 1][c_comp + 1] = 'C';
		grid[r_comp][c_comp] = '*';
		user--;
		done = true;
	}
	/*These conditions works when a random position of computer check is passed in the function:-
	Check whether user check can be killed by checking it with random position passed*/
	else if (r_comp - 2 >= 0 && c_comp - 2 >= 0 && grid[r_comp][c_comp] == 'C' && (grid[r_comp - 1][c_comp - 1] == '1' || grid[r_comp - 1][c_comp - 1] == 'P') && grid[r_comp - 2][c_comp - 2] == '*')    /*Makes move in the upper left diagonal*/
	{
		grid[r_comp - 2][c_comp - 2] == 'C';
		grid[r_comp - 1][c_comp - 1] == '*';
		grid[r_comp][c_comp] == '*';
		user--;
		done = true;
	}
	else if (r_comp - 2 >= 0 && c_comp + 2 < cols && grid[r_comp][c_comp] == 'C' && (grid[r_comp - 1][c_comp + 1] == '1' || grid[r_comp - 1][c_comp + 1] == 'P') && grid[r_comp - 2][c_comp + 2] == '*')              /*Makes move in the upper right diagonal*/
	{
		grid[r_comp - 2][c_comp + 2] == 'C';
		grid[r_comp - 1][c_comp + 1] == '*';
		grid[r_comp][c_comp] == '*';
		user--;
		done = true;
	}
	else if (r_comp - 1 >= 0 && grid[r_comp - 1][c_comp - 1] == '*')      /*Makes move in the upper left diagonal*/
	{
		grid[r_comp - 1][c_comp - 1] = 'C';
		grid[r_comp][c_comp] = '*';
		done = true;
	}
	else if (r_comp - 1 >= 0 && grid[r_comp - 1][c_comp + 1] == '*')   /*Makes move in the upper right diagonal*/
	{
		grid[r_comp - 1][c_comp + 1] = 'C';
		grid[r_comp][c_comp] = '*';
		done = true;
	}
}